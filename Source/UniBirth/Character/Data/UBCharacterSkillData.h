// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Common/Data/UBBuffData.h"
#include "Common/UBdefine.h"
#include "Component/UBBuffComponent.h"
#include "UBCharacterSkillData.generated.h"


















USTRUCT(BlueprintType)
struct FCharacterSkill : public FTableRowBase
{
    GENERATED_BODY()


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 skill_id;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString skill_key;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString skill_name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString skill_buffs;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPassiveCondition PassiveCondition;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECharacterType using_character;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString description;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 mp_cost;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 hitCount;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 target_scope;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTexture2D> skill_icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ENotifyType FirstKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxKeyLength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageRate;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FBuffSlot buff;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETeamType teamType;

};

UCLASS()
class UNIBIRTH_API UUBCharacterSkillData : public UDataAsset
{
	GENERATED_BODY()

public:


};
