// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UBMonsterSpawnPoint.generated.h"

UCLASS()
class UNIBIRTH_API AUBMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()

public:

	AUBMonsterSpawnPoint();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<class AUBBaseMonster>> MonsterClasses;
};
