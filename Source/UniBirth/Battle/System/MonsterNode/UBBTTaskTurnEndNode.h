// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBBTTaskTurnEndNode.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBBTTaskTurnEndNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UUBBTTaskTurnEndNode();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	
};
