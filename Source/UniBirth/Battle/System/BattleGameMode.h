// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Battle/BattleManager.h"
#include "Common/UBdefine.h"
#include "BattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	ABattleManager* battleManager = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSubclassOf<class AUBPlayer> UniPlayerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSubclassOf<AUBPlayer> TauPlayerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSubclassOf<AUBPlayer> ElvasiaPlayerClass;

public:
	UFUNCTION(BlueprintCallable)
	ABattleManager* GetBattleManager();

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AUBPlayer> GetPlayerClass(ECharacterType Type) const;


};
