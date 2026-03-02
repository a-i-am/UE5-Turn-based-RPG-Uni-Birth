// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/UBdefine.h"
#include "UI/Data/CharacterPortraitData.h"
#include "UBCharacterPortrait.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBCharacterPortrait : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* Portrait;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	ECharacterType charactertype;
	
	UFUNCTION(BlueprintCallable)
	void ReApplyPortrait();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<UCharacterPortraitData> portraits;
};
