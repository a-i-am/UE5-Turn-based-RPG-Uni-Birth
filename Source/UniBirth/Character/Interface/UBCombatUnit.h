// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Common/UBdefine.h"
#include "Character/Data/UBCharacterSkillData.h"
#include "UBCombatUnit.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnCutsceneFinished);
DECLARE_MULTICAST_DELEGATE(FOnActionFinished);
UCLASS()
class UNIBIRTH_API AUBCombatUnit : public ACharacter
{
	GENERATED_BODY()

public:


	AUBCombatUnit();
	virtual void AttackHit();

protected:

	virtual void BeginPlay() override;
public:

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsMyTurn = false;

	void SetTurn(bool bTurn) { bIsMyTurn = bTurn; }
	bool IsMyTurn() const { return bIsMyTurn; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETeamType teamType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USceneComponent> CameraPivot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECharacterType characterType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UUBStatsComponent> statsComp;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AUBCombatUnit> CurrentTarget;

	UFUNCTION(BlueprintCallable)
	AUBCombatUnit* GetCurrentUnit() const { return CurrentTarget; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UHealthComponent> HealthComp;

	UPROPERTY()
	FTimerHandle MovementTimerHandle;

	UPROPERTY()
	FTimerHandle ReturnTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<class UUBCharacterFXProfile> FXProfile;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EActionState currentState;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* floatingDamageArea;

	UPROPERTY()
	TObjectPtr<class UUBSkillManager> SkillManager;


	FCharacterSkill* FindSkillKey(FString keyName);
	FCharacterSkill* FindSkillId(int32 skillId);


	UPROPERTY()
	TArray<FCharacterSkill> OwnedSkills;

	UPROPERTY()
	TArray<FCharacterSkill> OwnedPassvieSkills;

	UFUNCTION()
	TArray<FCharacterSkill> GetSkillDataTable(){return OwnedSkills;}

	TObjectPtr<class UUBGameInstance> gameInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UUBBuffComponent> BuffComp;

	void PlayCutscene(class ULevelSequence*);

	UFUNCTION()
	void OnSequenceFinished();

	virtual void DeathCharacter();
	UPROPERTY()
	TObjectPtr<class ALevelSequenceActor> cachedSequenceActor;

	FVector OriginalLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UActionSystemComponent> asComp;

	UPROPERTY()
	TObjectPtr<class ABattleManager>  bm;


	UPROPERTY()
	TObjectPtr<class UUBDamageSubsystem> damageSST;

	virtual bool bIsDead();

	UFUNCTION(BlueprintCallable, Category = "Action")
	EActionState GetCurrentActionState() { return currentState; }

	void HealMp(const int value);
	bool IsStealthed();


	bool bIsDeath = false;
	virtual void Counter(AUBCombatUnit* unit);

	UFUNCTION()
	void PrintDamage(int damage);

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<class AUBDamageFloat> damageFloatUI;

	EResultType result;

	UFUNCTION(BlueprintCallable)
	void ActiveBuffEffect();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BuffEffect")
	class UNiagaraComponent* BurnEffect;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BuffEffect")
	class UNiagaraComponent* StunEffect;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BuffEffect")
	class UNiagaraComponent* PoisoningEffect;

	FOnCutsceneFinished OnCutSceneFinished;
	FOnActionFinished OnActionFinished_;
};
