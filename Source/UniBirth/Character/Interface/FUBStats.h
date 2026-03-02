#pragma once

#include "CoreMinimal.h"
#include "Common/UBdefine.h"
#include "FUBStats.generated.h"

USTRUCT(BlueprintType)
struct FUBStats : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
    FString CharacterDisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Hp = MaxHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxMp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Mp = MaxMp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Attack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Defence;
    //보호막
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 shield;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 speed; // 직접 타겟까지 가는 개체들에 적용(실제 전투에 유의미한 수치 X)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackSpeed; // 턴 가져오는 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float critRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResistRate;
 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 parryPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float dodgeTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 aggroPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EUnitState> CurrState;
};