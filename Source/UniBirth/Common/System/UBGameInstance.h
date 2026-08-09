#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Common/Data/UBItemData.h"
#include "Common/Data/UBBuffData.h"
#include "Character/Interface/FUBStats.h"
#include "Common/SubSystem/UBSkillManager.h"
#include "UBGameInstance.generated.h"

UCLASS()
class UNIBIRTH_API UUBGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table|Item")
	TObjectPtr<UDataTable> EquipmentData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table|Item")
	TObjectPtr<UDataTable> ConsumableData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table|Item")
	TObjectPtr<UDataTable> MaterialData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table|Skill")
	TObjectPtr<UDataTable> SkillData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table|Buff")
	TObjectPtr<UDataTable> ComboBuffData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table|Buff")
	TObjectPtr<UDataTable> SkillBuffData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table|Stat")
	TObjectPtr<UDataTable> StatData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table|Reward")
	TObjectPtr<UDataTable> RewardData;

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void GetSkillsForCharacter(ECharacterType InCharacterType, TArray<FCharacterSkill>& OutSkills, TArray<FCharacterSkill>& OutPassive);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	UTexture2D* GetSkillIcon(FString InSkillKey);

	FEquipmentStruct* GetEquipmentData(int32 InID);

	FConsumableStruct* GetConsumableData(int32 InID);

	FMaterialStruct* GetMaterialData(int32 InID);

	UFUNCTION(BlueprintCallable, Category = "Stat")
	FUBStats GetStatData(ECharacterType InCharacterType);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Manager")
	TObjectPtr<UUBSkillManager> SkillManager;

	FComboBuffData* GetComboBuffData(int32 InID);

	FSkillBuffData* GetSkillBuffData(int32 InID);

	UFUNCTION()
	void HandlePreMouseButtonDown(const FPointerEvent& InMouseEvent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Progress")
	int32 CurrentStage;

	UFUNCTION(BlueprintCallable, Category = "Game Progress")
	TArray<int32> GetReward(int32 InStage);
};

