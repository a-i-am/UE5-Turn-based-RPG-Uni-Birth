// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Common/UBdefine.h"
#include "UBAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API AUBAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AUBAIController();
	
	UPROPERTY(BlueprintReadOnly, Category = "Boss|Phase")
	EBossPhase currentPhase;

	EBossPhase GetPhase() const { return currentPhase; }


	UPROPERTY(EditAnyWhere,BlueprintReadOnly)
	TObjectPtr<class UBehaviorTreeComponent> behaviorComp;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class AUBCombatUnit> target;

	
	UPROPERTY()
	TObjectPtr<class UBlackboardComponent> ownerBlackboard; 
	
	virtual void OnPossess(APawn* pawn)  override;

	void UpdateToTarget();

	void SetCheckHandle();
	void FocusCameraOnTarget(AUBCombatUnit* Target);
};
