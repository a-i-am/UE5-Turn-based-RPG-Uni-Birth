// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "UBResetLocationNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBResetLocationNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float FrameDeltaTime) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation) override;

	FVector PrevPelvisWS = FVector::ZeroVector;
	bool bHasPrev = false;

	UPROPERTY(EditAnywhere, Category = "CoreMotion")
	bool bIgnoreZ = true;

	UPROPERTY(EditAnywhere, Category = "CoreMotion")
	FName PelvisBoneName = TEXT("Core");

	UPROPERTY(EditAnywhere, Category = "CoreMotion")
	float MotionScale = 0.8f;

	TObjectPtr<class ASampleCharacter> character;
};
