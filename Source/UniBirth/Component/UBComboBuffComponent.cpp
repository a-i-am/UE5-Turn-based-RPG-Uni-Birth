#include "UBComboBuffComponent.h"
#include "Engine/Engine.h"
#include "Common/System/UBGameInstance.h"
#include "Component/UBStatsComponent.h"
#include "UBBuffComponent.h"

UUBComboBuffComponent::UUBComboBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UUBComboBuffComponent::InitializeComponent()
{
	Super::InitializeComponent();

	ComboRandomStream.GenerateNewSeed();

	// GameInstance에 가면 ComboBuff Table 로드되어있다.
	UUBGameInstance* GameInstance = GetWorld()->GetGameInstance<UUBGameInstance>();
	if (GameInstance && GameInstance->ComboBuffData)
	{
		// 전체 모든 테이블을 읽어서, 재조립
		TArray<FComboBuffData*> Rows;
		GameInstance->ComboBuffData->GetAllRows(TEXT(""), Rows);

		for (FComboBuffData* Iter : Rows)
		{
			FComboLevelData* ComboTypeData = comboDataMap.Find(Iter->Combo);
			
			if (ComboTypeData)
			{
				{
					FComboStatusInfo statInfo;
					statInfo.statType = Iter->BuffStat;
					statInfo.op = Iter->StatOperator;
					statInfo.value = Iter->LevelC;	// value
					statInfo.ID = Iter->ID;
					ComboTypeData->levelMap[EComboLevel::C].statusCandidates.Add(statInfo);
				}
				{
					FComboStatusInfo statInfo;
					statInfo.statType = Iter->BuffStat;
					statInfo.op = Iter->StatOperator;
					statInfo.value = Iter->LevelB;	// value
					statInfo.ID = Iter->ID;
					ComboTypeData->levelMap[EComboLevel::B].statusCandidates.Add(statInfo);
				}
				{
					FComboStatusInfo statInfo;
					statInfo.statType = Iter->BuffStat;
					statInfo.op = Iter->StatOperator;
					statInfo.value = Iter->LevelA;	// value
					statInfo.ID = Iter->ID;
					ComboTypeData->levelMap[EComboLevel::A].statusCandidates.Add(statInfo);
				}
			}
			else
			{
				FComboLevelData newLevelData;

				// nullptr 은 한번도 추가한적 없다.
				{
					FComboStatusList statList;
					FComboStatusInfo statInfo;
					statInfo.statType = Iter->BuffStat;
					statInfo.op = Iter->StatOperator;
					statInfo.value = Iter->LevelC;	// value
					statInfo.ID = Iter->ID;
					statList.statusCandidates.Add(statInfo);
					newLevelData.levelMap.Add(EComboLevel::C, statList);
				}
				{
					FComboStatusList statList;
					FComboStatusInfo statInfo;
					statInfo.statType = Iter->BuffStat;
					statInfo.op = Iter->StatOperator;
					statInfo.value = Iter->LevelB;	// value
					statInfo.ID = Iter->ID;
					statList.statusCandidates.Add(statInfo);
					newLevelData.levelMap.Add(EComboLevel::B, statList);
				}
				{
					FComboStatusList statList;
					FComboStatusInfo statInfo;
					statInfo.statType = Iter->BuffStat;
					statInfo.op = Iter->StatOperator;
					statInfo.value = Iter->LevelA;	// value
					statInfo.ID = Iter->ID;
					statList.statusCandidates.Add(statInfo);
					newLevelData.levelMap.Add(EComboLevel::A, statList);
				}

				comboDataMap.Add(Iter->Combo, newLevelData);
			}
		}
	
		UUBBuffComponent* BuffComp = GetOwner()->GetComponentByClass<UUBBuffComponent>();
		if (BuffComp)
		{
			BuffComp->OnBuffForceRemoved.AddDynamic(this, &UUBComboBuffComponent::HandleBuffForceRemoved);
		}
	}
}

void UUBComboBuffComponent::HandleBuffForceRemoved(int32 RemovedTableID)
{
	for (auto& Elem : allActiveBuffs)
	{
		TArray<FActiveBuff>& BuffList = Elem.Value;
		for (int32 i = 0; i < BuffList.Num(); ++i)
		{
			if (BuffList[i].TableID == RemovedTableID)
			{
				BuffList.RemoveAt(i);
				UE_LOG(LogTemp, Warning, TEXT("Force Removed Combo Buff ID: %d"), RemovedTableID);
				return;
			}
		}
	}
}

void UUBComboBuffComponent::RequestAddComboBuff(EComboType InType)
{
	UUBBuffComponent* BuffComp = GetOwner()->GetComponentByClass<UUBBuffComponent>();
	if (!BuffComp)
	{
		return;
	}

	UUBGameInstance* GI = GetWorld()->GetGameInstance<UUBGameInstance>();
	
	if (!allActiveBuffs.Contains(InType))
	{
		allActiveBuffs.Add(InType, TArray<FActiveBuff>());
	}
	TArray<FActiveBuff>& ActiveBuffs = allActiveBuffs[InType];

	bool bHasA = false;
	bool bHasB = false;
	bool bHasC = false;

	for (const FActiveBuff& Buff : ActiveBuffs)
	{
		if (Buff.level == EComboLevel::A)
		{
			bHasA = true;
		}
		if (Buff.level == EComboLevel::B)
		{
			bHasB = true;
		}
		if (Buff.level == EComboLevel::C)
		{
			bHasC = true;
		}
	}

	if (bHasA && bHasB && bHasC)
	{
		return;
	}

	// 머지
	int32 MergeTargetIndex = FindMergeTargetIndex(InType, EComboLevel::C);
	if (MergeTargetIndex != -1)
	{
		RemoveActiveBuff(ActiveBuffs, MergeTargetIndex);
		FActiveBuff FinalBuff = GetFinalMergedBuff(InType, EComboLevel::B);

		// 추가될 버프는 머지된 결과인 최종 하나
		ActiveBuffs.Add(FinalBuff);
		ApplyToBuffComponent(FinalBuff);
	}
	else
	{
		// 머지가 아니라면 C등급 버프를 추가
		if (ActiveBuffs.Num() < maxSlotCount)
		{
			FActiveBuff NewBuff = CreateActiveBuff(InType, EComboLevel::C);
			ActiveBuffs.Add(NewBuff);
			ApplyToBuffComponent(NewBuff);
		}
	}

	ComboStatRateUpdate();
}
int32 UUBComboBuffComponent::FindMergeTargetIndex(EComboType inType, EComboLevel inLevel)
{
	if (TArray<FActiveBuff>* activeBuff = allActiveBuffs.Find(inType))
	{
		for (int32 i = 0; i < activeBuff->Num(); ++i)
		{
			if ((*activeBuff)[i].level == inLevel)
			{
				return i;
			}
		}
	}

	return -1; // 없음	
}

FActiveBuff UUBComboBuffComponent::GetFinalMergedBuff(EComboType Type, EComboLevel Level)
{
	int32 ExistIndex = FindMergeTargetIndex(Type, Level);

	if (ExistIndex != -1 && Level < EComboLevel::MAX)
	{
		TArray<FActiveBuff>& ActiveBuffs = allActiveBuffs[Type];
		RemoveActiveBuff(ActiveBuffs, ExistIndex);
		
		return GetFinalMergedBuff(Type, (EComboLevel)((uint8)Level + 1));
	}

	return CreateActiveBuff(Type, Level);
}
FActiveBuff UUBComboBuffComponent::CreateActiveBuff(EComboType Type, EComboLevel Level)
{
	FActiveBuff NewBuff;
	NewBuff.type = Type;
	NewBuff.level = Level;

	// 버프 타입과 레벨로 랜덤 스탯을 검색
	FComboStatusInfo* NewStatus = GetRandomStatus(Type, Level);

	// 검색되면
	if (NewStatus)
	{
		// 추가될 버프에 스탯 타입과 ID를 적용 
		NewBuff.stat = NewStatus->statType;
		NewBuff.TableID = NewStatus->ID;
		NewBuff.StatOp = NewStatus->op;
	}

	return NewBuff;
}

void UUBComboBuffComponent::ApplyToBuffComponent(const FActiveBuff& InBuff)
{
	UUBBuffComponent* BuffComp = GetOwner()->GetComponentByClass<UUBBuffComponent>();
	UUBGameInstance* GameInstance = GetWorld()->GetGameInstance<UUBGameInstance>();

	if (BuffComp && GameInstance && GameInstance->ComboBuffData)
	{
		FComboBuffData* TableData = GameInstance->GetComboBuffData(InBuff.TableID);
		if (TableData)
		{
			FBuffSlot NewSlot;
			NewSlot.ID = InBuff.TableID;
			
			NewSlot.priority = EBuffPriority::SurvivalCombo;
			NewSlot.CurrTurnCount = 0;
			NewSlot.TurnDurationCount = TableData->TurnDurationCount;
			NewSlot.DurType = TableData->DurType;
			NewSlot.BuffStats.Add(TableData->BuffStat);
			NewSlot.StatOperator = TableData->StatOperator;
			NewSlot.MakeDelCase(TableData->DelCase);

			if (InBuff.level == EComboLevel::C)
			{
				NewSlot.Icon = TableData->Icon_C;
				NewSlot.Values.Add(TableData->LevelC);
			}
			else if (InBuff.level == EComboLevel::B)
			{
				NewSlot.Icon = TableData->Icon_B;
				NewSlot.Values.Add(TableData->LevelB);
			}
			else if (InBuff.level == EComboLevel::A)
			{
				NewSlot.Icon = TableData->Icon_A;
				NewSlot.Values.Add(TableData->LevelA);
			}

			BuffComp->AddBuffToStat(NewSlot, false);
		}
	}
}

void UUBComboBuffComponent::ComboStatRateUpdate()
{
	// 콤보로 인한 스탯 배율 총합을 업데이트 
	UUBStatsComponent* Stats = GetOwner()->GetComponentByClass<UUBStatsComponent>();
	UUBBuffComponent* BuffComp = GetOwner()->GetComponentByClass<UUBBuffComponent>();
	if (!Stats || !BuffComp)
	{
		return;
	}
	
	FUBStats PreStats = Stats->currentStats;
	//Stats->ResetComboStats();

	for (auto& Elem : allActiveBuffs)
	{
		TMap<EBuffStatType, int32> FixedTotalMap;
		TMap<EBuffStatType, float> RateTotalMap;
		TMap<EBuffStatType, int32> StatRepresentativeID;

		for (const FActiveBuff& Buff : Elem.Value)
		{
			// 특정 버프 스탯이 전체 콤보데이터 슬롯에 이미 있는지 확인 => 키로 저장
			StatRepresentativeID.FindOrAdd(Buff.stat) = Buff.TableID; 

			// 스탯 계산 방식에 따라 다른 맵에 저장
			if (Buff.StatOp == EStatOperator::Static)
			{
				FixedTotalMap.FindOrAdd(Buff.stat) += (int32)Buff.Value;
			}
			else if (Buff.StatOp == EStatOperator::Rate)
			{
				RateTotalMap.FindOrAdd(Buff.stat) += Buff.Value;
			}
		}

		// 스탯 고정 값 적용 (기존 값에 레벨별 값이 그대로 더해지는 형태)
		for (auto& Pair : FixedTotalMap)
		{
			EBuffStatType TargetStat = Pair.Key;
			int32 TotalFixed = Pair.Value;

			Stats->UpdateComboStat(TargetStat, TotalFixed, 0.0f);

			if (BuffComp)
			{
				FBuffSlot TempSlot;
				TempSlot.ID = StatRepresentativeID[TargetStat];
				TempSlot.BuffStats.Add(TargetStat);
				TempSlot.Values.Add((float)TotalFixed);
				TempSlot.StatOperator = EStatOperator::Static;
			
				BuffComp->LogStatChange(GetOwner()->GetName(), TempSlot, PreStats, Stats->currentStats);
				PreStats = Stats->currentStats;
			}
		}

		// 스탯 비율 값 적용 (기존 값에 레벨별 값이 n.n 소수점(퍼센테이지) 비율 값으로 곱해진 후 기존값에 더해지는 형태)
		// 예시) 10 += 10 * 0.5 = 15
		for (auto& Pair : RateTotalMap)
		{
			EBuffStatType TargetStat = Pair.Key;
			float TotalRate = Pair.Value;

			Stats->UpdateComboStat(TargetStat, 0, TotalRate);

			if (BuffComp)
			{
				FBuffSlot TempSlot;

				TempSlot.ID = StatRepresentativeID[TargetStat];
				TempSlot.BuffStats.Add(TargetStat);
				TempSlot.Values.Add(TotalRate);
				TempSlot.StatOperator = EStatOperator::Rate;

				BuffComp->LogStatChange(GetOwner()->GetName(), TempSlot, PreStats, Stats->currentStats);
				PreStats = Stats->currentStats;
			}
		}
	}
}

float UUBComboBuffComponent::GetComboTotalRate(EComboType ComboType)
{
	float TotalRate = 0.0f;
	TArray<FActiveBuff>* BuffList = allActiveBuffs.Find(ComboType);

	if (BuffList == nullptr || BuffList->Num() == 0)
	{
		return 0.0f; // 버프가 없으면 0 반환
	}

	UUBGameInstance* GameInstance = GetWorld()->GetGameInstance<UUBGameInstance>();
	if (!GameInstance || !GameInstance->ComboBuffData)
	{
		return 0.0f; // 콤보버프 데이터를 못 가져오면
	}

	for (const FActiveBuff& Buff : *BuffList)
	{
		FComboBuffData* TableData = GameInstance->GetComboBuffData(Buff.TableID);
		if (TableData)
		{
			if (Buff.level == EComboLevel::C)
			{
				TotalRate += TableData->LevelC;
			}
			else if (Buff.level == EComboLevel::B)
			{
				TotalRate += TableData->LevelB;
			}
			else if (Buff.level == EComboLevel::A)
			{
				TotalRate += TableData->LevelA;
			}
		}
	}
	return TotalRate;
}

// 스테이터스 맵 내에서 랜덤 뽑기
FComboStatusInfo* UUBComboBuffComponent::GetRandomStatus(EComboType inType, EComboLevel inLevel)
{
	if (comboDataMap.Contains(inType))
	{
		FComboLevelData& lvData = comboDataMap[inType];
		if (lvData.levelMap.Contains(inLevel))
		{
			TArray<FComboStatusInfo>& candidates = lvData.levelMap[inLevel].statusCandidates;
			if (candidates.Num() > 0)
			{
				int32 randIdx = ComboRandomStream.RandRange(0, candidates.Num() - 1);
				return &candidates[randIdx];
			}
		}
	}

	// 데이터가 없거나 실수했을 때 기본값 (혹은 Error Status)
	return nullptr;
}
void UUBComboBuffComponent::RemoveActiveBuff(TArray<FActiveBuff>& BuffList, int32 Index)
{
	UUBBuffComponent* BuffComp = GetOwner()->GetComponentByClass<UUBBuffComponent>();
	if (BuffComp)
	{
		FBuffSlot DeleteSlot;
		DeleteSlot.ID = BuffList[Index].TableID;
		DeleteSlot.priority = EBuffPriority::SurvivalCombo;

		// 정확한 삭제를 위해 아이콘 정보를 찾아 채워넣기.
		UUBGameInstance* GameInstance = GetWorld()->GetGameInstance<UUBGameInstance>();
		if (GameInstance && GameInstance->ComboBuffData)
		{
			FComboBuffData* TableData = GameInstance->GetComboBuffData(DeleteSlot.ID);
			if (TableData)
			{
				EComboLevel LevelToRemove = BuffList[Index].level;

				if (LevelToRemove == EComboLevel::C)
				{
					DeleteSlot.Icon = TableData->Icon_C;
				}
				else if (LevelToRemove == EComboLevel::B)
				{
					DeleteSlot.Icon = TableData->Icon_B;
				}
				else if (LevelToRemove == EComboLevel::A)
				{
					DeleteSlot.Icon = TableData->Icon_A;
				}
			}
		}

		BuffComp->DeleteBuff(DeleteSlot);
	}

	// 기존 슬롯 1개를 제거하고, 합쳐진 버프 1개를 새로 추가
	BuffList.RemoveAt(Index);
}

void UUBComboBuffComponent::ResetBuffs(EComboType type, EDelCase reason)
{
	TArray<FActiveBuff>* activeBuff = allActiveBuffs.Find(type);
	if (!activeBuff || activeBuff->Num() == 0)
	{
		return;
	}

	EBuffStatType TargetStat = (*activeBuff)[0].stat;
	UUBBuffComponent* BuffComp = GetOwner()->GetComponentByClass<UUBBuffComponent>();

	if (BuffComp)
	{
		for (const auto& buffs : *activeBuff)
		{
			FBuffSlot DeleteSlot;
			DeleteSlot.ID = buffs.TableID;
			DeleteSlot.priority = EBuffPriority::SurvivalCombo;
			BuffComp->DeleteBuff(DeleteSlot);
		}
	}

	activeBuff->Empty();

	UUBStatsComponent * Stats = GetOwner()->GetComponentByClass<UUBStatsComponent>();
	if (Stats)
	{
		Stats->UpdateComboStat(TargetStat, 0.0f, 0);
	}

	ComboStatRateUpdate();
}