// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "UBBTCanSkillDecorator.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBBTCanSkillDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UUBBTCanSkillDecorator();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;


	UPROPERTY(EditAnywhere)
	int32 RequiredMP = 2;
};
UCLASS()
class UNIBIRTH_API UUBBTCheckGameStart : public UBTDecorator
{
	GENERATED_BODY()

public:
	UUBBTCheckGameStart();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
