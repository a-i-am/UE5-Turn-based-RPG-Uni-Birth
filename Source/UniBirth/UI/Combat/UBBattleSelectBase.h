// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UBBattleSelectBase.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBBattleSelectBase : public UUserWidget 
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PressQ();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PressW();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PressE();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PressA();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PressS();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PressD();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PressSpace();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isActiveWidget = true;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayAnim();
	UFUNCTION(BlueprintCallable)
	void CloseWidget();
};
