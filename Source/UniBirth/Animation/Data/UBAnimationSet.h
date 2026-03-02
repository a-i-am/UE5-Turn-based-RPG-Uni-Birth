// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "UBAnimationSet.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAnimActionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag ActionTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> Montage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<class UUBSequenceData> cutSecne;

};

UCLASS(BlueprintType)
class UUBAnimationSet : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAnimActionData> Actions;

    const FAnimActionData* FindData(FGameplayTag Tag);
   

};
