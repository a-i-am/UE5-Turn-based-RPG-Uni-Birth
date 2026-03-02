// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Data/CharacterPortraitData.h"
#include "UBBattleTargetSelectButton.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBBattleTargetSelectButton : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AUBCombatUnit* TargetUnit;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* TargetImage;

	UFUNCTION(BlueprintCallable)
	void SetButtonImage();
	UFUNCTION(BlueprintCallable)
	void Init(AUBCombatUnit* Unit);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCharacterPortraitData> textures;
};
