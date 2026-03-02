// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Common/UBdefine.h"
#include "Character/Data/UBCharacterSkillData.h"
#include "BattleManager.generated.h"

UENUM(BlueprintType)
enum class EBattleState : uint8
{
	None,
	Wait,
	Turn,
	Die,
};
class AUBCombatUnit;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMonsterAttackStarted,
	AUBCombatUnit*  /*attack */, FCharacterSkill* skill /* Target*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(
	FOnMonsterAttackResult,
	AUBCombatUnit* /*Attacker*/,
	AUBCombatUnit* /*Target*/,
	EResultType /*Result*/
);
DECLARE_MULTICAST_DELEGATE(FOnUIHide);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHitAction, AUBCombatUnit* /*Target*/, EResultType);
DECLARE_MULTICAST_DELEGATE(FOnTimeDelayEnded);
UCLASS()
class UNIBIRTH_API ABattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	void InitBattle();
	void StartBattle();
	void NextTurn();
	void ResetCharacters();
	bool CheckGameEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BuildTurnOrder();

	void BuildOrderList();

	void StartNewRound();

	UPROPERTY(EditAnywhere)
	TArray<class AUBCombatUnit*> characters;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AUBCombatUnit>> TurnOrder;       

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AUBCombatUnit>> CurrentTurnOrder;

	
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AUBCombatUnit>> TotalTurnOrder;
	
	AUBCombatUnit* currentCharacter = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UUBCharacterPortraitList> turnOrderListWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 currentTurn = 0;
	UPROPERTY(EditAnywhere)
	int32 currentTurnIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AUBCombatUnit>> targetList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AUBCombatUnit>> PlayerList;


	UPROPERTY(EditAnywhere)
	int32 currentTargetIndex = 0;
	UFUNCTION(BlueprintCallable)
	TArray<AUBCombatUnit*> GetTargetList();
	TArray<AUBCombatUnit*> GetSingleTargetPlayerList();
	UFUNCTION(BlueprintCallable)
	AUBCombatUnit* GetCurrentCharacter() { return currentCharacter; }

	AUBCombatUnit* GetRandomPlayer();
	const TArray<AUBCombatUnit*>& GetAllPlayer();
	FOnMonsterAttackStarted OnMonsterAttackStarted;
	FOnMonsterAttackResult OnMonsterAttackResult;
	FOnHitAction OnHitAciton;
	FOnTimeDelayEnded OnTimeDelayEnded;
	EResultType CurrentResult = EResultType::Success;
	EResultType GetReactionResult() const { return CurrentResult; }

	bool bIsProcessingTurn = false;

	UFUNCTION()
	void HandleMonsterAttackResult(class AUBBaseMonster* Monster, AUBCombatUnit* Target,EResultType Result);
	void BrodacastAttackResult(EResultType Result);
	void BroadcastAttackResultToSingle(AUBCombatUnit* Target, EResultType Result);

	void SetReactionResult(EResultType InResult);

	void ForceSetActionState(EActionState State);

	void DeathCharacterReMove(AUBCombatUnit* deathUnit);

	UFUNCTION(BlueprintCallable)
	void GlobalTimeStop();

	UFUNCTION(BlueprintCallable)
	void UIHideAllWiget();

	FOnUIHide OnUIHide;
	bool bIsTimeDelay = true;
	UFUNCTION(BlueprintCallable)
	void SetGlobalTimeReset();
	bool bIsStun(AUBCombatUnit* unit);

	UFUNCTION(BlueprintNativeEvent)
	void GameOver();
	virtual void GameOver_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void GameClear();
	virtual void GameClear_Implementation();

	void HandleGameEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class UCameraComponent> TargetCamera;

	void TargetAttack(AUBCombatUnit* target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
	TObjectPtr<class ULevelSequence> sequenceCut;

};
