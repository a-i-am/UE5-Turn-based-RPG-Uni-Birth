// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "FBossPhaseEffect.generated.h"  
USTRUCT(BlueprintType)
struct FBossPhaseEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AttackMultiplier = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DefenseMultiplier = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DamageTakenMultiplier = 1.f;
};
