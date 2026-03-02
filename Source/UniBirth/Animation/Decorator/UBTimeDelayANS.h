// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "UBTimeDelayANS.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBTimeDelayANS : public UAnimNotifyState
{
	GENERATED_BODY()
public:
    virtual void NotifyBegin(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation,
        float TotalDuration
    ) override;

    virtual void NotifyTick(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation,
        float FrameDeltaTime
    ) override;

    virtual void NotifyEnd(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation
    ) override;

    UPROPERTY()
    TObjectPtr<class ABattleManager> BM;

    TObjectPtr<class UWorld> World;
};
