// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBChangePhaseTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBChangePhaseTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UUBChangePhaseTaskNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<class ULevelSequence> Sequence;

	

	void HandleCutsceneFinished(UBehaviorTreeComponent* OwnerComp);

	UPROPERTY()
	AActor* CachedViewTarget = nullptr;


};
