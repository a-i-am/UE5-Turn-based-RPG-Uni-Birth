// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UBRewardTable.generated.h"

USTRUCT(BlueprintType)
struct FRewardStruct : public FTableRowBase

{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RandomCase;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RandomWeight;
};
/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBRewardTable : public UDataAsset
{
	GENERATED_BODY()
	
};
