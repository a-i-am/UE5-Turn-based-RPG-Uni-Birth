// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UBSequenceData.generated.h"

/**
 * 
 */
//@TODO
UCLASS()
class UNIBIRTH_API UUBSequenceData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class ULevelSequence> Sequence;

	// 쓸지 말지 여부 . 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseGlobalSlowmotion = false;
	//슬로모션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bUseGlobalSlowmotion"))
	float GlobalTimeDilation = 0.3f;
};
