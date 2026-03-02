// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Common/UBdefine.h"
#include "UBHPCheckDecorator.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBHPCheckDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	UUBHPCheckDecorator();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
	EBossPhase currentBossPhase;

	UPROPERTY(EditAnywhere)
	float RequiredHp = 0.8f;

};

UCLASS()
class UNIBIRTH_API UUBMpCheckDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	UUBMpCheckDecorator();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;



	UPROPERTY(EditAnywhere)
	int32 RequiredMp = 5;

};

UCLASS()
class UNIBIRTH_API UUBCheckStatsDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	UUBCheckStatsDecorator();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;



	UPROPERTY(EditAnywhere)
	EUnitState RequiredStats;

};

UCLASS()
class UNIBIRTH_API UUBCheckLastSkillDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	UUBCheckLastSkillDecorator();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;


	UPROPERTY(EditAnywhere)
	FGameplayTag lastSkillTag;

	UPROPERTY(EditAnywhere)
	int32 range;


};
