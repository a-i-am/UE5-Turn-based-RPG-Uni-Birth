// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/Spawn/UBSpawnPoint.h"

// Sets default values
AUBSpawnPoint::AUBSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUBSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUBSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

