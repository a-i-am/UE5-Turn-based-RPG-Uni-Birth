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


	UUBGameInstance* GameInstance = GetWorld()->GetGameInstance<UUBGameInstance>();
	if (GameInstance && GameInstance->ComboBuffData)
	{

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
					statInfo.value = Iter->LevelC;
					statInfo.ID = Iter->ID;
					ComboTypeData->levelMap[EComboLevel::C].statusCandidates.Add(statInfo);
				}
				{
					FComboStatusInfo statInfo;
					statInfo.statType = Iter->BuffStat;
					statInfo.op = Iter->StatOperator;
					statInfo.value = Iter->LevelB;
					statInfo.ID = Iter->ID;
					ComboTypeData->levelMap[EComboLevel::B].statusCandidates.Add(statInfo);
				}
				{
					FComboStatusInfo statInfo;
					statInfo.statType = Iter->BuffStat;
					statInfo.op = Iter->StatOperator;
					statInfo.value = Iter->LevelA;
					statInfo.ID = Iter->ID;
					ComboTypeData->levelMap[EComboLevel::A].statusCandidates.Add(statInfo);
				}
			}
			else
			{
				FComboLevelData newLevelData;


				{
					FComboStatusList statList;
					FComboStatusInfo statInfo;
					statInfo.statType = Iter->BuffStat;
					statInfo.op = Iter->StatOperator;
					statInfo.value = Iter->LevelC;
					statInfo.ID = Iter->ID;
					statList.statusCandidates.Add(statInfo);
					newLevelData.levelMap.Add(EComboLevel::C, statList);
				}
				{
					FComboStatusList statList;
					FComboStatusInfo statInfo;
					statInfo.statType = Iter->BuffStat;
					statInfo.op = Iter->StatOperator;
					statInfo.value = Iter->LevelB;
					statInfo.ID = Iter->ID;
					statList.statusCandidates.Add(statInfo);
					newLevelData.levelMap.Add(EComboLevel::B, statList);
				}
				{
					FComboStatusList statList;
					FComboStatusInfo statInfo;
					statInfo.statType = Iter->BuffStat;
					statInfo.op = Iter->StatOperator;
					statInfo.value = Iter->LevelA;
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


	int32 MergeTargetIndex = FindMergeTargetIndex(InType, EComboLevel::C);
	if (MergeTargetIndex != -1)
	{
		RemoveActiveBuff(ActiveBuffs, MergeTargetIndex);
		FActiveBuff FinalBuff = GetFinalMergedBuff(InType, EComboLevel::B);


		ActiveBuffs.Add(FinalBuff);
		ApplyToBuffComponent(FinalBuff);
	}
	else
	{

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

	return -1;
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


	FComboStatusInfo* NewStatus = GetRandomStatus(Type, Level);


	if (NewStatus)
	{

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

	UUBStatsComponent* Stats = GetOwner()->GetComponentByClass<UUBStatsComponent>();
	UUBBuffComponent* BuffComp = GetOwner()->GetComponentByClass<UUBBuffComponent>();
	if (!Stats || !BuffComp)
	{
		return;
	}

	FUBStats PreStats = Stats->currentStats;


	for (auto& Elem : allActiveBuffs)
	{
		TMap<EBuffStatType, int32> FixedTotalMap;
		TMap<EBuffStatType, float> RateTotalMap;
		TMap<EBuffStatType, int32> StatRepresentativeID;

		for (const FActiveBuff& Buff : Elem.Value)
		{

			StatRepresentativeID.FindOrAdd(Buff.stat) = Buff.TableID;


			if (Buff.StatOp == EStatOperator::Static)
			{
				FixedTotalMap.FindOrAdd(Buff.stat) += (int32)Buff.Value;
			}
			else if (Buff.StatOp == EStatOperator::Rate)
			{
				RateTotalMap.FindOrAdd(Buff.stat) += Buff.Value;
			}
		}


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
		return 0.0f;
	}

	UUBGameInstance* GameInstance = GetWorld()->GetGameInstance<UUBGameInstance>();
	if (!GameInstance || !GameInstance->ComboBuffData)
	{
		return 0.0f;
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
