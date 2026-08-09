// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/System/UBGameInstance.h"
#include "Character/Data/UBCharacterSkillData.h"
#include "Common/Data/UBSaveGame.h"
#include "Common/SubSystem/UBSkillManager.h"
#include "Kismet/GameplayStatics.h"
#include "Common/Data/UBRewardTable.h"

void UUBGameInstance::Init()
{
	Super::Init();

	SkillManager = NewObject<UUBSkillManager>(this);



	if (UUBSaveGame* LoadedGame = Cast<UUBSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainData"), 0)))
	{
		CurrentStage = LoadedGame->CurrentStage;
	}
}

void UUBGameInstance::HandlePreMouseButtonDown(const FPointerEvent& MouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("@@ Global Click Event"));
}

TArray<int32> UUBGameInstance::GetReward(int32 InStage)
{
	TArray<int32> ResultItemIDs;

	if (!RewardData)
		return ResultItemIDs;

	TMap<int32, TArray<FRewardStruct>> RandomCaseMap;

	static const FString Context(TEXT("RewardContext"));
	TArray<FRewardStruct*> Rows;
	RewardData->GetAllRows(Context, Rows);

	for (FRewardStruct* Row : Rows)
	{
		if (!Row || Row->Level != InStage)
			continue;

		if (Row->RandomCase == 0)
		{
			ResultItemIDs.Add(Row->ItemID);
		}
		else
		{
			RandomCaseMap.FindOrAdd(Row->RandomCase).Add(*Row);
		}
	}

	for (auto& Pair : RandomCaseMap)
	{
		const TArray<FRewardStruct>& Rewards = Pair.Value;

		int32 TotalWeight = 0;
		for (const FRewardStruct& Reward : Rewards)
		{
			TotalWeight += Reward.RandomWeight;
		}

		if (TotalWeight <= 0)
			continue;

		int32 RandValue = FMath::RandRange(1, TotalWeight);

		int32 AccWeight = 0;
		for (const FRewardStruct& Reward : Rewards)
		{
			AccWeight += Reward.RandomWeight;

			if (RandValue <= AccWeight)
			{
				ResultItemIDs.Add(Reward.ItemID);
				break;
			}
		}
	}

	return ResultItemIDs;
}

void UUBGameInstance::GetSkillsForCharacter(ECharacterType InCharacterType, TArray<FCharacterSkill>& OutSkills, TArray<FCharacterSkill>& OutPassive)
{
	OutSkills.Empty();
	OutPassive.Empty();
	TArray<FCharacterSkill*> Rows;
	SkillData->GetAllRows(TEXT("Load"), Rows);
	for (auto Row : Rows)
	{
		if (Row && Row->using_character != InCharacterType)
		{
			continue;
		}
		if (Row->skill_key.Contains(TEXT("SKILL_PASSIVE")))
			OutPassive.Add(*Row);
		else
			OutSkills.Add(*Row);
	}
}

UTexture2D* UUBGameInstance::GetSkillIcon(FString InSkillKey)
{
	FCharacterSkill* Row = SkillData->FindRow<FCharacterSkill>(
		FName(InSkillKey),
		TEXT("GetSkillIcon")
	);
	if (Row == nullptr) return nullptr;
	if (Row->skill_icon != nullptr)
		return Row->skill_icon.Get();

	return Row->skill_icon;
}

FEquipmentStruct* UUBGameInstance::GetEquipmentData(int32 InID)
{
	if (EquipmentData == nullptr)
	{
		return nullptr;
	}

	return EquipmentData->FindRow<FEquipmentStruct>(*FString::FromInt(InID), TEXT(""));
}

FConsumableStruct* UUBGameInstance::GetConsumableData(int32 InID)
{
	if (ConsumableData == nullptr)
	{
		return nullptr;
	}

	return ConsumableData->FindRow<FConsumableStruct>(*FString::FromInt(InID), TEXT(""));
}

FMaterialStruct* UUBGameInstance::GetMaterialData(int32 InID)
{
	if (MaterialData == nullptr)
	{
		return nullptr;
	}

	return MaterialData->FindRow<FMaterialStruct>(*FString::FromInt(InID), TEXT(""));
}

FUBStats UUBGameInstance::GetStatData(ECharacterType InCharacterType)
{
	FUBStats empty;
	if (StatData == nullptr)
	{
		return empty;
	}

	const UEnum* EnumPtr = StaticEnum<ECharacterType>();
	FString EnumString = EnumPtr->GetNameStringByValue((int64)InCharacterType);
	FName RowName = FName(*EnumString);

	switch (InCharacterType)
	{
	case ECharacterType::Uni:
		RowName = FName(TEXT("Uni"));
		break;
	case ECharacterType::Tau:
		RowName = FName(TEXT("Tau"));
		break;
	case ECharacterType::Elvasia:
		RowName = FName(TEXT("Elvasia"));
		break;
	case ECharacterType::Common:
		RowName = FName(TEXT("Celestial"));
		break;
	case ECharacterType::Elite:
		RowName = FName(TEXT("Basilisk"));
		break;
	case ECharacterType::Boss:
		RowName = FName(TEXT("Perses"));
		break;
	default:
		return empty;
	}

	FUBStats* stats = StatData->FindRow<FUBStats>(RowName, TEXT(""));
	return (stats) ? *stats : empty;
}

FComboBuffData* UUBGameInstance::GetComboBuffData(int32 InID)
{
	if (ComboBuffData == nullptr)
	{
		return nullptr;
	}

	return ComboBuffData->FindRow<FComboBuffData>(*FString::FromInt(InID), TEXT(""));
}

FSkillBuffData* UUBGameInstance::GetSkillBuffData(int32 InID)
{
	if (SkillBuffData == nullptr)
	{
		return nullptr;
	}

	return SkillBuffData->FindRow<FSkillBuffData>(*FString::FromInt(InID), TEXT(""));
}

