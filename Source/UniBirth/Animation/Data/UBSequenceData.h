// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UBSequenceData.generated.h"





UCLASS()
class UNIBIRTH_API UUBSequenceData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class ULevelSequence> Sequence;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseGlobalSlowmotion = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bUseGlobalSlowmotion"))
	float GlobalTimeDilation = 0.3f;
};
