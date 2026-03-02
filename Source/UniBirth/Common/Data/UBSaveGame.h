// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Common/UBdefine.h"
#include "Common/SubSystem/UBInventorySubsystem.h"
#include "UBSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBSaveGame : public USaveGame
{
	GENERATED_BODY()
private:
	UUBSaveGame();
public:
	UPROPERTY(VisibleAnywhere)
	TArray<ECharacterType> SelectedCharacters;

	UPROPERTY(VisibleAnywhere)
	TArray<FInventoryStruct> Inventory;

	UPROPERTY(BlueprintReadOnly)
	FCharacterEquipmentSaveStruct UniEquipment;
	UPROPERTY(BlueprintReadOnly)
	FCharacterEquipmentSaveStruct TauEquipment;
	UPROPERTY(BlueprintReadOnly)
	FCharacterEquipmentSaveStruct ElvasiaEquipment;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentStage;


};
