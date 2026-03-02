// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UBHitAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBHitAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UUBHitAnimNotify();
	// Fill out your copyright notice in the Description page of Project Settings.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
