// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/Data/UBAnimationSet.h"
#include "NiagaraFunctionLibrary.h"
#include "UBCharacterFXProfile.generated.h"


USTRUCT(BlueprintType)
struct FCharacterActionFXOverride
{
	GENERATED_BODY()


    UPROPERTY(EditAnywhere)
    FGameplayTag ActionTag;   

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UParticleSystem> Particle = nullptr;

    // Niagara
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UNiagaraSystem> NiagaraFX = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ImpactScale = FVector(10.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator ImpactRotator = FRotator(10.f);
};
UCLASS()
class UUBCharacterFXProfile : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FCharacterActionFXOverride> FXList;

    const FCharacterActionFXOverride* Find(FGameplayTag tag) const
    {
        for (const auto& E : FXList)
        {
            if (E.ActionTag == tag)
                return &E;
        }
        return nullptr;
    }
};
