// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UBBattleMainUI.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBBattleMainUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UUBCharacterPortraitList* CharacterPortraitList;

	UFUNCTION(BlueprintImplementableEvent)
	void AddMonsterHPBar();
};
