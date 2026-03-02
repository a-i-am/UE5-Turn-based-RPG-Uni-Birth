// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Common/Data/UBBuffData.h"
#include "Common/UBdefine.h"
#include "Component/UBBuffComponent.h"
#include "UBCharacterSkillData.generated.h"

//USTRUCT(BlueprintType)
//struct FSkillEffect
//{
//    GENERATED_BODY()
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite)
//    EEffectType EffectType = EEffectType::None;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite)
//    float Value = 0.f;
//    //턴
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite)
//    int32 Duration = 0;
//
//};

USTRUCT(BlueprintType)
struct FCharacterSkill : public FTableRowBase
{
    GENERATED_BODY()

    // 스킬 고유 ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 skill_id;

    // 내부 식별 키
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString skill_key;

    // 인게임 표시 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString skill_name;
    // 스킬 수치 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString skill_buffs;

    // 패시브조건 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPassiveCondition PassiveCondition;

    // 사용 캐릭터 ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECharacterType using_character;

    // 인게임 설명
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString description;

    // 소모 MP
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 mp_cost;

    // 공격 횟수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 hitCount;

    // 타겟 범위
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 target_scope;

    // 아이콘
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTexture2D> skill_icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ENotifyType FirstKey;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxKeyLength;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageRate;

    // 근거리 원거리 
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
                            
    //UTexture2D* GetSkillicon() { return }
};
