// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "UBBTTaskAttackNode.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBBTTaskAttackNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UUBBTTaskAttackNode();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void OnActionFinished(UBehaviorTreeComponent* OwnerComp);
public:
	/*UPROPERTY(EditAnyWhere, BluePrintReadOnly)
	TArray<FGameplayTag>actionTag ;*/

	UPROPERTY(EditAnyWhere, BluePrintReadOnly)
	FGameplayTag lastActionTag;

	UPROPERTY(EditAnywhere, BluePrintReadOnly)
	bool AgroTarget = false;
};
