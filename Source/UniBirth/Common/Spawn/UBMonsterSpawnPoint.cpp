// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/Spawn/UBMonsterSpawnPoint.h"

// Sets default values
AUBMonsterSpawnPoint::AUBMonsterSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUBMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUBMonsterSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

