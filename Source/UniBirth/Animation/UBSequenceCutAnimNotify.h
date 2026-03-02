// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UBSequenceCutAnimNotify.generated.h"

/**
 * 
 */
class ULevelSequencePlayer;

UCLASS()
class UNIBIRTH_API UUBSequenceCutAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	/*UUBSequenceCutAnimNotify();*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cutscene")
	TObjectPtr<class ULevelSequence> Sequence;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


};
