// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UBCharacterPortrait.h"
#include "UBCharacterPortraitList.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBCharacterPortraitList : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UUBCharacterPortrait*> portraitlist;

	UPROPERTY(meta = (BindWidget))
	UUBCharacterPortrait* CharacterPortrait_1;
	UPROPERTY(meta = (BindWidget))
	UUBCharacterPortrait* CharacterPortrait_2;
	UPROPERTY(meta = (BindWidget))
	UUBCharacterPortrait* CharacterPortrait_3;
	UPROPERTY(meta = (BindWidget))
	UUBCharacterPortrait* CharacterPortrait_4;
	UPROPERTY(meta = (BindWidget))
	UUBCharacterPortrait* CharacterPortrait_5;
	UPROPERTY(meta = (BindWidget))
	UUBCharacterPortrait* CharacterPortrait_6;
	//UPROPERTY(meta = (BindWidget))
	//UUBCharacterPortrait* CharacterPortrait_7;
	//UPROPERTY(meta = (BindWidget))
	//UUBCharacterPortrait* CharacterPortrait_8;

	UFUNCTION(BlueprintCallable)
	void ReApplyAll();
	UFUNCTION(BlueprintCallable)
	void SetPortraitType(int index, ECharacterType type);
};
