// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Common/UBdefine.h"
#include "UBItemData.generated.h"

USTRUCT(BlueprintType)
struct FMaterialStruct : public FTableRowBase

{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Icon;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEquipmentGrade EquipmentGrade;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Amount;
};

USTRUCT(BlueprintType)
struct FEquipmentStruct : public FTableRowBase

{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Icon;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEquipmentType EquipmentType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEquipmentGrade EquipmentGrade;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECharacterType Weapontype;
};

USTRUCT(BlueprintType)
struct FConsumableStruct : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 maxCount;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 maxUseCount;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Buff;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Icon;

};



UCLASS()
class UNIBIRTH_API UUBItemData : public UDataAsset
{
	GENERATED_BODY()

};
