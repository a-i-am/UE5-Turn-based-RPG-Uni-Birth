// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UBTService_IncreaseAttackPerTurn.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBTService_IncreaseAttackPerTurn : public UBTService
{
	GENERATED_BODY()
public:
	UUBTService_IncreaseAttackPerTurn();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool bAlreadyIncreased = false;
};
