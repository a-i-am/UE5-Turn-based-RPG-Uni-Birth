// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Common/UBdefine.h"
#include "UBDamageFloat.generated.h"

UCLASS()
class UNIBIRTH_API AUBDamageFloat : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUBDamageFloat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetDamageText(int32 damage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDefenceAction(EActionState state);
};
