// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UBBattleHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API AUBBattleHUD : public AHUD
{
	GENERATED_BODY()
private:
	AUBBattleHUD();
protected:
	virtual void BeginPlay() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainUIClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UUserWidget> MainUIWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class AUBCharacterUI> CharacterUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class AUBCharacterUI> CharacterUI;

	UPROPERTY(BlueprintReadOnly)
	AUBCharacterUI* CharacterUIActor;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> ScreenGuardWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* ScreenGuardWidget;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MonsterHPClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UUserWidget> MonsterHPWidget;

	UPROPERTY(BlueprintReadWrite)
	class UUBBattleSelectBase* CurrentWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	bool bIsAngry;


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowScreenGuardUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideScreenGuardUI();

	UFUNCTION()
	void OnTimeDelayEnded_Internal();

	UFUNCTION(BlueprintCallable)
	bool GetIsAngry() { return bIsAngry; }

	UFUNCTION(BlueprintCallable)
	void InitHUD();
};

