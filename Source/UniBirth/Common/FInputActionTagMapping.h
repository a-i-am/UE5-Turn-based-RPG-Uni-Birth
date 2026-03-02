// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "FInputActionTagMapping.generated.h"

USTRUCT(BlueprintType)
struct FInputActionTagMapping
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TObjectPtr<UInputAction> InputAction;

    UPROPERTY(EditAnywhere)
    FGameplayTag ActionTag;
};