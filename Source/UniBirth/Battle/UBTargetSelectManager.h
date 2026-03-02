// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UBTargetSelectManager.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBTargetSelectManager : public UObject
{
	GENERATED_BODY()
	
 public:

    void Init(class ABattleManager* InBattleManager);

    void Activate();          // 턴 시작 시 활성화
    void Deactivate();        // 턴 종료 시 비활성화

    void MoveTarget(int32 Direction);  // A/D
    void ConfirmTarget();              // Enter

private:

    void UpdateHighlight();   // 시각적 선택 갱신
    int32 FindFirstEnemyIndex();       // 자동 선택

private:
    UPROPERTY()
    TObjectPtr<class ABattleManager> battleManager;   
    UPROPERTY()
    int32 currentIndex;
    bool bActive;                   

};
