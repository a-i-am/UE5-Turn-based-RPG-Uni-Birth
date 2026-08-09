#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Common/UBdefine.h"
#include "Character/Interface/UBCombatUnit.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "Animation/Data/FBossPhaseEffect.h"
#include "UBBaseMonster.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnActionFinishedSignature);

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
	FCharacterSkill* ResolveSkillDataFromActionTag(const FGameplayTag& InActionTag);
	void InterpRotateTo(const FRotator& InTargetRot, float InSpeed, FTimerHandle& InTimerHandle);
	void StartAttackSequence(FGameplayTag InActionTag, AUBCombatUnit* InTargetUnit);
	void StartAttack();

	virtual void AttackHit() override;
	void MonsterOnActionFinished();
	void NotifyAttackIntent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ECurrentAttackType CurrentAttackType;

	void AllPlayerIdle();
	void MoveToTarget();
	void StartMove();
	void StartReturnMove();
	void BeforeLocation();

	void StartRotateToTarget(AUBCombatUnit* InTargetUnit);
	void RotateTickToTarget();
	void RotateTickToOrigin();
	void StartRotateMeshToOrigin();

	FCharacterSkill* CurrentSkillData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<TObjectPtr<AUBCombatUnit>> ResolvedTargets;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	const TArray<AUBCombatUnit*>& GetResolvedTargets() const { return ResolvedTargets; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	FGameplayTag CurrentActionTag;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	bool bActionStarted = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsAttack = false;

	FVector MoveTargetLocation;
	FRotator OriginRotator;
	FRotator TargetRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotateSpeed = 200.f;

	FTimerHandle RotateTimerHandle;
	FTimerHandle RotateToOriginTimerHandle;

	FOnActionFinishedSignature OnActionFinishedDelegate;
	void HandleShieldBroken();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsPlaying = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Monster")
	EMonsterType MonsterType;

	UPROPERTY(BlueprintReadOnly, Category = "Boss|Phase")
	EBossPhase CurrentPhase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Phase")
	TMap<EBossPhase, FBossPhaseEffect> PhaseEffectMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<FGameplayTag> LastSkillList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsRefill = false;

	UFUNCTION(BlueprintCallable, Category = "Shield")
	void ShieldRefill();

	virtual void DeathCharacter() override;
};

