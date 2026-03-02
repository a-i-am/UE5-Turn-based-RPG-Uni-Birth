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

	//FSlateApplication::Get().OnApplicationMousePreInputButtonDownListener().AddUObject(this, &UUBGameInstance::HandlePreMouseButtonDown);

	if (UUBSaveGame* LoadedGame = Cast<UUBSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainData"), 0)))
	{
		CurrentStage = LoadedGame->CurrentStage;
	}
}

void UUBGameInstance::HandlePreMouseButtonDown(const FPointerEvent& MouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("@@ Global Click Event"));
}

TArray<int32> UUBGameInstance::GetReward(int32 stage)
{
	TArray<int32> ResultItemIDs;

	if (!RewardData)
		return ResultItemIDs;

	TMap<int32, TArray<FRewardStruct>> RandomCaseMap;

	// 1️DataTable 순회
	static const FString Context(TEXT("RewardContext"));
	TArray<FRewardStruct*> Rows;
	RewardData->GetAllRows(Context, Rows);

	for (FRewardStruct* Row : Rows)
	{
		if (!Row || Row->Level != stage)
			continue;

		if (Row->RandomCase == 0)
		{
			// 고정 보상
			ResultItemIDs.Add(Row->ItemID);
		}
		else
		{
			// 랜덤 보상 후보
			RandomCaseMap.FindOrAdd(Row->RandomCase).Add(*Row);
		}
	}

	// 2️ RandomCase별 가중치 랜덤
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

void UUBGameInstance::GetSkillsForCharacter(ECharacterType CharacterID, TArray<FCharacterSkill>& OutSkills,
	TArray<FCharacterSkill>&OutPassvie)
{
	//스킬 비우고
	OutSkills.Empty();
	OutPassvie.Empty();
	TArray<FCharacterSkill*> Rows;
	SkillData->GetAllRows(TEXT("Load"), Rows);
	for (auto Row : Rows)
	{
		if (Row && Row->using_character != CharacterID)
		{
			continue;
		}
		if (Row->skill_key.Contains(TEXT("SKILL_PASSIVE")))
			OutPassvie.Add(*Row);
		else
			OutSkills.Add(*Row);
	}
}

UTexture2D* UUBGameInstance::GetSkillIcon(FString SKillKey)
{
	FCharacterSkill* Row = SkillData->FindRow<FCharacterSkill>(
		FName(SKillKey),
		TEXT("GetSkillIcon")
	);
	if (Row == nullptr)return nullptr;
	if (Row->skill_icon!=nullptr)
		return Row->skill_icon.Get();

	return Row->skill_icon;
}

FEquipmentStruct* UUBGameInstance::GetEquipmentData(int32 ID)
{
	if (EquipmentData == nullptr)
	{
		return nullptr;
	}

	return EquipmentData->FindRow<FEquipmentStruct>(*FString::FromInt(ID), TEXT(""));
}

FConsumableStruct* UUBGameInstance::GetConsumableData(int32 ID)
{
	if (ConsumableData == nullptr)
	{
		return nullptr;
	}

	return ConsumableData->FindRow<FConsumableStruct>(*FString::FromInt(ID), TEXT(""));
}

FMaterialStruct* UUBGameInstance::GetMaterialData(int32 ID)
{
	if (MaterialData == nullptr)
	{
		return nullptr;
	}

	return MaterialData->FindRow<FMaterialStruct>(*FString::FromInt(ID), TEXT(""));
}

FUBStats UUBGameInstance::GetStatData(ECharacterType characterType)
{
	FUBStats empty;
	if (StatData == nullptr)
	{
		return empty;
	}
	
	const UEnum* EnumPtr = StaticEnum<ECharacterType>();
	FString EnumString = EnumPtr->GetNameStringByValue((int64)characterType);
	FName RowName = FName(*EnumString);
	
	switch (characterType)
	{
	case ECharacterType::CT_Uni:
		RowName = FName(TEXT("Uni"));
		break;
	case ECharacterType::CT_Tau:
		RowName = FName(TEXT("Tau"));
		break;
	case ECharacterType::CT_Elvasia:
		RowName = FName(TEXT("Elvasia"));
		break;
	case ECharacterType::CT_Common:
		RowName = FName(TEXT("Celestial"));
		break;
	case ECharacterType::CT_Elite:
		RowName = FName(TEXT("Basilisk"));
		break;
	case ECharacterType::CT_Boss:
		RowName = FName(TEXT("Perses"));
		break;
	default:
		return empty;
	}

	FUBStats* stats = StatData->FindRow<FUBStats>(RowName, TEXT(""));
	return (stats) ? *stats : empty;
}

FComboBuffData* UUBGameInstance::GetComboBuffData(int32 ID)
{
	if (ComboBuffData == nullptr)
	{
		return nullptr;
	}

	return ComboBuffData->FindRow<FComboBuffData>(*FString::FromInt(ID), TEXT(""));
}

FSkillBuffData* UUBGameInstance::GetSkillBuffData(int32 ID)
{
	if (SkillBuffData == nullptr)
	{
		return nullptr;
	}

	return SkillBuffData->FindRow<FSkillBuffData>(*FString::FromInt(ID), TEXT(""));
}
