// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UBMotageEndAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBMotageEndAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UUBMotageEndAnimNotify();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
