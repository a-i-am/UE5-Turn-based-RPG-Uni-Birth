#include "Component/UBBuffComponent.h"
#include "Battle/BattleManager.h"
#include "Battle/System/BattleGameMode.h"
#include "Character/Interface/UBCombatUnit.h"
#include "Component/UBStatsComponent.h"

UUBBuffComponent::UUBBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts1
void UUBBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ABattleGameMode* GM = GetWorld()->GetAuthGameMode<ABattleGameMode>())
	{
		if (GM == nullptr)
		{
			return;
		}

		BM = GM->GetBattleManager();
		if (BM == nullptr)
		{
			return;
		}
	}
}

void UUBBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UUBBuffComponent::AddBuffToStat(FBuffSlot NewBuff, bool bApplyStat)
{
	//10개의 버프를 이미 보유하고 있는 캐릭터에게 새로운 버프가 부여될 경우
	//Add되기 전에, 낮은 우선도의 맨 앞자리를 제거해줘야함

	// 기존에 동일한 ID의 버프가 있는지 확인
	FBuffSlot* ExistingBuff = nullptr;

	if (CurrBuffs[(int32)NewBuff.priority].Num() > 0)
	{
		for (FBuffSlot& Slot : CurrBuffs[(int32)NewBuff.priority])
		{
			if (Slot.ID == NewBuff.ID)
			{
				ExistingBuff = &Slot;
				break;
			}
		}
	}

	if (ExistingBuff)
	{
		switch (ExistingBuff->StackRule)
		{
		case EBuffStackRule::ResetTurnCount:
			ExistingBuff->CurrTurnCount = 0;

			if (onBuffChangedDynamic.IsBound())
			{
				onBuffChangedDynamic.Broadcast();
			}
			return; // 새 슬롯을 만들지 않고 종료

		case EBuffStackRule::CalculateSum:
			if (ExistingBuff->CurrentStack < ExistingBuff->MaxStackCount)
			{
				if (bApplyStat)
				{
					ApplyStat(*ExistingBuff, -1);
				}
				
				// 값 합산 및 스택 증가(배열의 모든 수치를 각각 합산)
				for (int32 i = 0; i < ExistingBuff->Values.Num(); ++i)
				{
					if (NewBuff.Values.IsValidIndex(i))
					{
						ExistingBuff->Values[i] += NewBuff.Values[i];
					}
				}
				
				ExistingBuff->CurrentStack++;
				
				if (bApplyStat)
				{
					ApplyStat(*ExistingBuff, 1);
				}

				if (onBuffChangedDynamic.IsBound())
				{
					onBuffChangedDynamic.Broadcast();
				}
				return;
			}
		case EBuffStackRule::None:
			return;
		}
	}

 	int32 currBuffsNum = GetTotalNum();
	if (currBuffsNum >= slotMax)
	{
		MakeSlot();
	}
	
	NewBuff.BaseValues = NewBuff.Values;
	NewBuff.CurrentStack = 1;

	// 버프추가
	EBuffPriority TargetType = NewBuff.priority;
	CurrBuffs[(int32)TargetType].Add(NewBuff);
	
	if (bApplyStat)
	{
		ApplyStat(NewBuff, 1);
	}

	if (onBuffChangedDynamic.IsBound())
	{
		onBuffChangedDynamic.Broadcast();
	}
}

void UUBBuffComponent::DeleteBuff(FBuffSlot targetBuff)
{
	EBuffPriority TargetType = targetBuff.priority;
	TArray<FBuffSlot>& Arr = CurrBuffs[(int32)TargetType]; // 특정타입을 보관하는 배열. 그 안에서 순회

	// 삭제할 대상이 정해져있으니까 아이디로 검색
	for (int i = 0; i < Arr.Num(); i++)
	{
		if (Arr[i].ID == targetBuff.ID)
		{
			ApplyStat(Arr[i], -1);
			Arr.RemoveAt(i);
			break;
		}
	}

	if (onBuffChangedDynamic.IsBound())
	{
		onBuffChangedDynamic.Broadcast();
	}
}

void UUBBuffComponent::DeleteBuffByReason(EDelCase reason)
{
	for (int32 priority = (int32)EBuffPriority::SurvivalCombo; priority <= (int32)EBuffPriority::Item; ++priority)
	{
		CurrBuffs[priority].RemoveAll([this, reason](const FBuffSlot& Slot)
			{
				for (EDelCase Case : Slot.DelCase)
				{
					if (Case == reason)
					{
						ApplyStat(Slot, -1);
						return true;
					}
				}
				
				return false;
			});
	}
}

void UUBBuffComponent::MakeSlot()
{
	int32 currBuffsNum = GetTotalNum();
	if (currBuffsNum == 0)
	{
		return;
	}

	// 낮은 우선도의 맨 앞자리를 제거
	for (int32 i = (int32)EBuffPriority::Item; i >= 0; --i)
	{
		if (CurrBuffs[i].Num() != 0)
		{
			if (OnBuffForceRemoved.IsBound())
			{
				OnBuffForceRemoved.Broadcast(CurrBuffs[i][0].ID);
			}

			CurrBuffs[i].RemoveAt(0);
			return;
		}
	}
}

int32 UUBBuffComponent::GetTotalNum()
{
	int32 TotalNum = CurrBuffs[0].Num() + CurrBuffs[1].Num() + CurrBuffs[2].Num();
	return TotalNum;
}

void UUBBuffComponent::RemoveTurnExpiredBuffs()
{
	// RemoveAll 내부의 람다 함수([this](const FBuffSlot& Slot))는 배열에 요소가 존재하고 조건 검사가 시작될 때만 진입
	// 만약 첫 번째 배열이 비어있다면, if (Slot.DurType == EBuffDurType::.. 이 부분 중단점까지 닿지 x)

	// 0~2번까지 모든 우선순위 배열을 명확히 순회
	for (int32 i = 0; i < 3; ++i)
	{
		int32 RemovedCount = CurrBuffs[i].RemoveAll([this](const FBuffSlot& Slot)
		{
			if (Slot.DurType == EBuffDurType::TurnDuration && Slot.CurrTurnCount >= Slot.TurnDurationCount)
			{
				ApplyStat(Slot, -1);
				return true;
			}
			return false;
		});

		if (RemovedCount > 0)
		{
			onBuffChangedDynamic.Broadcast();
		}
	}
}

TArray<UTexture2D*> UUBBuffComponent::GetAllIcon() const
{
	// 버프 아이콘들이 모여있는 공통 폴더
	const FString BasePath = TEXT("/Game/Assets/Texture/UI/Battle/BuffIcons/");

	TArray<UTexture2D*> OutArray;
 	for (int32 priority = (int32)EBuffPriority::SurvivalCombo; priority <= (int32)EBuffPriority::Item; ++priority)
	{
		for (int32 index = 0; index < CurrBuffs[priority].Num(); ++index)
		{
			// Object Path 생성
			// /Game/UI/Icons/Sword_Icon.Sword_Icon
			const FString FullPath = BasePath + CurrBuffs[priority][index].Icon + TEXT(".") + CurrBuffs[priority][index].Icon;

			UTexture2D* Texture = LoadObject<UTexture2D>(
				nullptr,
				*FullPath
			);

			if (!Texture)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to load icon: %s"), *FullPath);
			}
		
			OutArray.Add(Texture);
		}
	}

	return OutArray;
}

void UUBBuffComponent::ApplyStat(const FBuffSlot& newBuff, int32 Op)
{
	if (newBuff.priority == EBuffPriority::SurvivalCombo)
	{
		return;
	}

	AUBCombatUnit* Unit = Cast<AUBCombatUnit>(GetOwner());
	UUBStatsComponent* stat = Unit->statsComp;

	if (!Unit || !stat)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Buff ID %d : Stats Num %d, Values Num %d"), newBuff.ID, newBuff.BuffStats.Num(), newBuff.Values.Num());

	FUBStats PreStats = stat->currentStats;
	
	if (newBuff.DurType == EBuffDurType::Permanent || newBuff.DurType == EBuffDurType::StackPerTurn || newBuff.DurType == EBuffDurType::TurnDuration)
	{
		for (int32 i = 0; i < newBuff.BuffStats.Num(); ++i)
		{
			EBuffStatType CurrentStatType = newBuff.BuffStats[i];
			float CurrentValue = newBuff.Values.IsValidIndex(i) ? newBuff.Values[i] : 0.0f;

			switch (CurrentStatType)
			{
			case EBuffStatType::MaxHp:
			{
				if (newBuff.StatOperator == EStatOperator::Static)
				{
					stat->ApplyMaxHP(Op * CurrentValue, 0);
				}
				else if (newBuff.StatOperator == EStatOperator::Rate)
				{
					stat->ApplyMaxHP(0, Op * CurrentValue);
				}
			}
			break;
			case EBuffStatType::Hp:
			{
				if (newBuff.StatOperator == EStatOperator::Static)
				{
					stat->ApplyHPheal(Op * CurrentValue, 0);
				}
				else if (newBuff.StatOperator == EStatOperator::Rate)
				{
					stat->ApplyHPheal(0, Op * CurrentValue);
				}
			}
			break;
			case EBuffStatType::Mp:
			{
				if (newBuff.StatOperator == EStatOperator::Static)
				{
					stat->ApplyMPHeal(Op * CurrentValue, 0);
				}
				else if (newBuff.StatOperator == EStatOperator::Rate)
				{
					stat->ApplyMPHeal(0, Op * CurrentValue);
				}
			}
			break;
			case EBuffStatType::Attack:
			{
				if (newBuff.StatOperator == EStatOperator::Static)
				{
					stat->ApplyAttackStats(Op * CurrentValue, 0);
				}
				else if (newBuff.StatOperator == EStatOperator::Rate)
				{
					stat->ApplyAttackStats(0, Op * CurrentValue);
				}
			}
			break;
			case EBuffStatType::Defence:
			{
				if (newBuff.StatOperator == EStatOperator::Static)
				{
					stat->ApplyDefence(Op * CurrentValue, 0);
				}
				else if (newBuff.StatOperator == EStatOperator::Rate)
				{
					stat->ApplyDefence(0, Op * CurrentValue);
				}
			}
			break;
			case EBuffStatType::Shield:
			{
				if (newBuff.StatOperator == EStatOperator::Static)
				{
					stat->ApplyShield(Op * CurrentValue, 0);
				}
				else if (newBuff.StatOperator == EStatOperator::Rate)
				{
					stat->ApplyShield(0, Op * CurrentValue);
				}
			}
			break;
			case EBuffStatType::Speed:
			{
				if (newBuff.StatOperator == EStatOperator::Static)
				{
					stat->ApplySpeedStats(Op * CurrentValue, 0);
				}
				else if (newBuff.StatOperator == EStatOperator::Rate)
				{
					stat->ApplySpeedStats(0, Op * CurrentValue);
				}
			}
			break;
			case EBuffStatType::CriticalRate:
				if (newBuff.StatOperator == EStatOperator::Static)
				{
					stat->ApplyCritRate(Op * CurrentValue, 0);
				}
				else if (newBuff.StatOperator == EStatOperator::Rate)
				{
					stat->ApplyCritRate(0, Op * CurrentValue);
				}
				break;
			case EBuffStatType::StateResist:
				if (newBuff.StatOperator == EStatOperator::Static)
				{
					stat->ApplyStateResist(Op * CurrentValue, 0);
				}
				else if (newBuff.StatOperator == EStatOperator::Rate)
				{
					stat->ApplyStateResist(0, Op * CurrentValue);
				}
				break;
			case EBuffStatType::ParryPoint:
				if (newBuff.StatOperator == EStatOperator::Static)
				{
					stat->ApplyParryPoint(Op * CurrentValue, 0);
				}
				else if (newBuff.StatOperator == EStatOperator::Rate)
				{
					stat->ApplyParryPoint(0, Op * CurrentValue);
				}
				break;
			case EBuffStatType::DodgeTime:
				if (newBuff.StatOperator == EStatOperator::Static)
				{
					stat->ApplyDodgeTime(Op * CurrentValue, 0);
				}
				else if (newBuff.StatOperator == EStatOperator::Rate)
				{
					stat->ApplyDodgeTime(0, Op * CurrentValue);
				}
				break;
			case EBuffStatType::AggroPoint:
				if (newBuff.StatOperator == EStatOperator::Static)
				{
					stat->ApplyAggroPoint(Op * CurrentValue, 0);
				}
				else if (newBuff.StatOperator == EStatOperator::Rate)
				{
					stat->ApplyAggroPoint(0, Op * CurrentValue);
				}
				break;
			case EBuffStatType::State:
				if (newBuff.State != EUnitState::None)
				{
					if (Op == -1)
					{
						stat->currentStats.CurrState.Remove(newBuff.State);
					}
					else
					{
						if (!stat->currentStats.CurrState.Contains(newBuff.State))
						{
							stat->currentStats.CurrState.Add(newBuff.State);
						}
					}
					Unit->ActiveBuffEffect();
				}
				break;
			}
		}
	}
	FString CharacterName = Unit->GetName(); // 플레이어 이름
	LogStatChange(CharacterName, newBuff, PreStats, stat->currentStats);
}

void UUBBuffComponent::AdvanceBuffTurns()
{
	for (int32 priority = 0; priority < 3; ++priority)
	{
		for (FBuffSlot& Slot : CurrBuffs[priority])
		{
			// 매 턴 시작 시 효과가 추가됨
			if (Slot.DurType == EBuffDurType::StackPerTurn)
			{
				if (Slot.CurrentStack < Slot.MaxStackCount)
				{
					ApplyStat(Slot, -1);

					for (int32 i = 0; i < Slot.Values.Num(); ++i)
					{
						if (Slot.BaseValues.IsValidIndex(i))
						{
							// 효과 중첩
							Slot.Values[i] += Slot.BaseValues[i];
						}
					}
					Slot.CurrentStack++;

					ApplyStat(Slot, 1);
				}
			}

			// 지정된 턴 수 만큼만 유지
			if (Slot.DurType == EBuffDurType::TurnDuration)
			{
				Slot.CurrTurnCount++;
			}
		}
	}
	RemoveTurnExpiredBuffs();
}

void UUBBuffComponent::LogStatChange(const FString& CharName, const FBuffSlot& Buff, const FUBStats& Before, const FUBStats& After)
{
#if WITH_EDITOR
	// 예: "[Player_1][Buff ID:3001]"
	FString BuffPrefix = FString::Printf(TEXT("[%s][Buff ID:%d]"), *CharName, Buff.ID);

	for (TFieldIterator<FProperty> Proplt(FUBStats::StaticStruct()); Proplt; ++Proplt)
	{
		FProperty* Prop = *Proplt;
		FString PropName = Prop->GetName();
		const void* BeforePtr = Prop->ContainerPtrToValuePtr<void>(&Before);
		const void* AfterPtr = Prop->ContainerPtrToValuePtr<void>(&After);
		
		// float
		if (FFloatProperty* FloatProp = CastField<FFloatProperty>(Prop))
		{
			float ValBefore = FloatProp->GetFloatingPointPropertyValue(BeforePtr);
			float ValAfter = FloatProp->GetFloatingPointPropertyValue(AfterPtr);

			if (FMath::Abs(ValBefore - ValAfter) > 0.01f)
			{
				FString LogMsg = FString::Printf(TEXT("%s %s : %.1f->%.1f (Diff: %+.1f)"), *BuffPrefix, *PropName, ValBefore, ValAfter, (ValAfter - ValBefore));
				GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::White, LogMsg);
				UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMsg);
			}
		}
		// int32
		else if (FIntProperty* IntProp = CastField<FIntProperty>(Prop))
		{
			int32 ValBefore = IntProp->GetSignedIntPropertyValue(BeforePtr);
			int32 ValAfter = IntProp->GetSignedIntPropertyValue(AfterPtr);

			if (ValBefore != ValAfter)
			{
				FString LogMsg = FString::Printf(TEXT("%s %s : %d->%d (Diff: %+d)"), *BuffPrefix, *PropName, ValBefore, ValAfter, (ValAfter - ValBefore));
				GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Cyan, LogMsg);
				UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMsg);
			}
		}
		// Enum(상태 이상)
		else if (FEnumProperty* EnumProp = CastField<FEnumProperty>(Prop))
		{
			const void* ValAddrBefore = EnumProp->ContainerPtrToValuePtr<void>(&Before);
			const void* ValAddrAfter = EnumProp->ContainerPtrToValuePtr<void>(&After);
			int64 ValBefore = EnumProp->GetUnderlyingProperty()->GetSignedIntPropertyValue(ValAddrBefore);
			int64 ValAfter =  EnumProp->GetUnderlyingProperty()->GetSignedIntPropertyValue(ValAddrAfter);

			if (ValBefore != ValAfter)
			{
				FString NameBefore = EnumProp->GetEnum()->GetNameStringByValue(ValBefore);
				FString NameAfter = EnumProp->GetEnum()->GetNameStringByValue(ValAfter);
				
				FString LogMsg = FString::Printf(TEXT("%s %s : %s->%s"), *BuffPrefix, *PropName, *NameBefore, *NameAfter);
				GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Orange, LogMsg);
				UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMsg);
			}
		}
	}
#endif
}

void FBuffSlot::MakeDelCase(FString DelCaseStr)
{
	DelCase.Empty();

	TArray<FString> OutArray;
	DelCaseStr.ParseIntoArray(OutArray, TEXT(","), true);

	UEnum* EnumPtr = StaticEnum<EDelCase>();
	for (int i = 0; i < OutArray.Num(); i++)
	{
		int64 EnumValue = EnumPtr->GetValueByName(FName(*OutArray[i]));

		// 본인 Slot의 결과에 추가
		DelCase.Add((EDelCase)EnumValue);
	}
}
