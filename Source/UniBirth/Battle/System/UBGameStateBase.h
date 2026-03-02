// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UBGameStateBase.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnGameStart)
UCLASS()
class UNIBIRTH_API AUBGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SetGameStart();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGameStart = false;

	FOnGameStart OnGameStart;

	UPROPERTY(BlueprintReadWrite)
	bool bAllowPlayerUI = false;

};
