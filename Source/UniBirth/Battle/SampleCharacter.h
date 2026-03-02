// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UBPlayer.h"
#include "SampleCharacter.generated.h"

UCLASS()
class UNIBIRTH_API ASampleCharacter : public AUBPlayer
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASampleCharacter();
	virtual bool operator<(const ASampleCharacter& Other) const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	FString name = TEXT("name");

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<class UInputMappingContext> inputMapping;
	UFUNCTION(BlueprintCallable)
	void Attack(AUBCombatUnit* unit);

	UFUNCTION(BlueprintCallable)
	void ActiveSkill(AUBCombatUnit* unit);

	UFUNCTION(BlueprintCallable)
	void UltiMateKSill(AUBCombatUnit* unit);

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	int32 NormalAttackHitCount = 1;
	

	UFUNCTION(BlueprintCallable)
	void OnActionFinished();
	void ResultGuard(EResultType Result);
	void ResultParrying(EResultType Result);
	void ResultDodge(EResultType Result);
	void Counter(AUBCombatUnit* Target);

	
	

	void StartParryingAction(AUBCombatUnit* unit, EResultType result);
	FVector InitialMeshRelativeLocation;
	FRotator InitialMeshRelativeRotation;
	
	UFUNCTION(BlueprintCallable)
	void Defence();
	UFUNCTION(BlueprintCallable)
	void SkipTurn();
	UFUNCTION(BlueprintCallable)
	void RotateToTarget(AUBCombatUnit* Unit);
	
	FRotator SavedRotBeforeAttack;
	bool bRestoreRotationAfterAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class AUBCharacterUI> CharacterUI;

	FGameplayTag CurrentActionTag;
	
	//UFUNCTION(BlueprintCallable, Category = "Action")
	//EActionState GetCurrentActionState() { return currentState; }
	virtual void AttackHit() override;
	//void SetTurn(bool IsTurn) { bIsMyTurn = IsTurn; };
	void HandleMonsterAttackResolved(AUBCombatUnit* Attacker,AUBCombatUnit* Target,EResultType Result);
	UFUNCTION(BlueprintCallable)
	void SetActionState(EActionState State);

	void MoveTowardTargetFromSocket(const FVector& CurrentSocketWS, AUBCombatUnit* Target, float DeltaTime);


	virtual void DeathCharacter() override;

	void CharacterResetLocation(const FVector& CurrentSocketWS,
		float DeltaTime);

};
	