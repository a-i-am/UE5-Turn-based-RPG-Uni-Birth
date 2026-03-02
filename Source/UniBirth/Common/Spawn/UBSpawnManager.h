// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UBSpawnManager.generated.h"

UCLASS()
class UNIBIRTH_API AUBSpawnManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUBSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;




	void SpawnMonster();

	UPROPERTY()
	TArray<AActor*> PlayerSpawnPoints;

	UPROPERTY()
	TArray<AActor*> monsterSpawnPoints;
	void SpawnPlayer();
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<class AUBPlayer>> PlayerClass;

};
