// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Common/UBdefine.h"
#include "UBAttackStartAnimNotify.generated.h"

///**
 //*
 //*/
UCLASS()
class UNIBIRTH_API UUBAttackStartAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
