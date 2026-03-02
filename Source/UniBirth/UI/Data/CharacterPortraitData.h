// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Common/UBdefine.h"
#include "CharacterPortraitData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterPortrait
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ECharacterType CharacterType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UTexture2D> Texture;
};

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UCharacterPortraitData : public UDataAsset
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FCharacterPortrait> Portraits;

    const FCharacterPortrait* FindData(ECharacterType type);
};
