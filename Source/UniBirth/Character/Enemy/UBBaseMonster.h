// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Common/UBdefine.h"
#include "Character/Interface/UBCombatUnit.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "Animation/Data/FBossPhaseEffect.h"
#include "UBBaseMonster.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnActionFinished);

UCLASS()
class UNIBIRTH_API AUBBaseMonster : public AUBCombatUnit
{
	GENERATED_BODY()

public:

	AUBBaseMonster();


protected:

	virtual void BeginPlay() override;


public:

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ResolveTargetsFromSkill();
	FCharacterSkill* ResolveSkillDataFromActionTag(const FGameplayTag& ActionTag);
	void InterpRotateTo(const FRotator& TargetRot, float Speed, FTimerHandle& TimerHandle);
	void StartAttackSequence(FGameplayTag ActionTag, AUBCombatUnit* target);

	void startAttack();

	virtual void AttackHit() override;
	void MonsterOnActionFinished();
	void NotifyAttackIntent();

	//UFUNCTION(BlueprintCallable, BlueprintPure)
	//EActionState GetActionState() const { return currentState; }



	UPROPERTY()
	ECurrentAttackType CurrentAttackType;

	void AllPlayerIdle();
	void MoveToTarget();
	void StartMove();

	void StartReturnMove();
	void BeforeLocation();


	void StartRotateToTarget(AUBCombatUnit* Target);
	void RotateTickToTarget();

	void RotateTickToOrigin();

	void StartRotateMeshToOrigin();

	 FCharacterSkill* CurrentSkillData;

	UPROPERTY()
	TArray<AUBCombatUnit*> ResolvedTargets;
	UFUNCTION(BlueprintCallable, Category = "Combat")
	const TArray<AUBCombatUnit*>& GetResolvedTargets() const { return ResolvedTargets; }
	
	FGameplayTag CurrentActionTag;

	UPROPERTY(BlueprintReadWrite)
	bool bActionStarted = false;

	bool bIsAttack = false;

	
	FVector MoveTargetLocation;
	FRotator OriginRotator;
	FRotator TargetRotator;

	float rotateSpeed = 200.f;
	FTimerHandle RotateTimerHandle;
	FTimerHandle RotatoeToOrginTimerHandle;

	FOnActionFinished OnActionFinishedDelegate;
	void HandleShieldBroken();
	
	bool bIsPlay = false;


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EMonsterType monsterType;

	UPROPERTY(BlueprintReadOnly, Category = "Boss|Phase")
	EBossPhase currentPhase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Phase")
	TMap<EBossPhase, FBossPhaseEffect> PhaseEffectMap;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	//TObjectPtr<USceneComponent> CameraPivot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBehaviorTree> BehaviorTree;


	UPROPERTY(EditAnyWhere, BluePrintReadOnly)
	TArray<FGameplayTag> lastSkillList;


	bool bisRefill = false;
	UFUNCTION(BlueprintCallable)
	 void SheildRefill();
	

	virtual void DeathCharacter() override;

};
