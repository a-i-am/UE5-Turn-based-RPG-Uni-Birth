// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UBPreAttackIndicatorLocation.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBPreAttackIndicatorLocation : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TObjectPtr <class UParticleSystem> FX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TObjectPtr <class UNiagaraSystem> NiagaraParticle;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	void SpawnParticle();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	FVector FXScale = FVector(1.f);	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	FRotator FXRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	FVector FXLocation = FVector(1.f);

	

};

	

