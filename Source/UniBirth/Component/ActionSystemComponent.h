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
	// Sets default values for this component's properties
	UActionSystemComponent();
	void OnActionSelected(FGameplayTag ActionTag);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UFUNCTION()
	UUBAnimInstance* GetUBAnimInstance();
	UAnimMontage* AttackMontage;




	
};
