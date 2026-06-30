// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UBTargetSelectManager.generated.h"




UCLASS()
class UNIBIRTH_API UUBTargetSelectManager : public UObject
{
	GENERATED_BODY()

 public:

    void Init(class ABattleManager* InBattleManager);

    void Activate();
    void Deactivate();

    void MoveTarget(int32 Direction);
    void ConfirmTarget();

private:

    void UpdateHighlight();
    int32 FindFirstEnemyIndex();

private:
    UPROPERTY()
    TObjectPtr<class ABattleManager> battleManager;
    UPROPERTY()
    int32 currentIndex;
    bool bActive;

};
