// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "ActionSystemComponent.generated.h"
class UBlackboardComponent;
class UBehaviorTreeComponent;
class UBehaviorTree;
class UUBAnimInstance;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIBIRTH_API UActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UActionSystemComponent();
	void OnActionSelected(FGameplayTag ActionTag);

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UUBAnimInstance* GetUBAnimInstance();
	UAnimMontage* AttackMontage;





};
