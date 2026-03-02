// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Common/Data/UBItemData.h"
#include "Common/Data/UBBuffData.h"
#include "Character/Interface/FUBStats.h"
#include "Common/SubSystem/UBSkillManager.h"
#include "UBGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	UDataTable* EquipmentData;

	UPROPERTY(EditAnywhere)
	UDataTable* ConsumableData;

	UPROPERTY(EditAnywhere)
	UDataTable* MaterialData;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillData;
	
	UPROPERTY(EditAnywhere)
	UDataTable* ComboBuffData;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillBuffData;
	
	UPROPERTY(EditAnywhere)
	UDataTable* StatData;

	UPROPERTY(EditAnywhere)
	UDataTable* RewardData;

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void GetSkillsForCharacter(ECharacterType CharacterID, TArray<FCharacterSkill>& OutSkills, TArray<FCharacterSkill>& OutPassvie);
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetSkillIcon(FString SKillKey);


	FEquipmentStruct* GetEquipmentData(int32 ID);

	FConsumableStruct* GetConsumableData(int32 ID);

	FMaterialStruct* GetMaterialData(int32 ID);
	
	UFUNCTION(BlueprintCallable)
	FUBStats GetStatData(ECharacterType characterType);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UUBSkillManager> SkillManager;

	FComboBuffData* GetComboBuffData(int32 ID);

	FSkillBuffData* GetSkillBuffData(int32 ID);

	UFUNCTION()
	void HandlePreMouseButtonDown(const FPointerEvent& MouseEvent);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStage;

	UFUNCTION(BlueprintCallable)
	TArray<int32> GetReward(int32 stage);
};
