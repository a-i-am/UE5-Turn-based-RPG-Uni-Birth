// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UBSpawnPoint.generated.h"

UCLASS()
class UNIBIRTH_API AUBSpawnPoint : public AActor
{
	GENERATED_BODY()

public:

	AUBSpawnPoint();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

};
