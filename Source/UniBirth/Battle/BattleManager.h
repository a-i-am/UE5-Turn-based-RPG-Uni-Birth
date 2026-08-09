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
	Die
};

class AUBCombatUnit;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMonsterAttackStartedSignature, AUBCombatUnit*, FCharacterSkill*);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnMonsterAttackResultSignature, AUBCombatUnit*, AUBCombatUnit*, EResultType);
DECLARE_MULTICAST_DELEGATE(FOnUIHideSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHitActionSignature, AUBCombatUnit*, EResultType);
DECLARE_MULTICAST_DELEGATE(FOnTimeDelayEndedSignature);

UCLASS()
class UNIBIRTH_API ABattleManager : public AActor
{
	GENERATED_BODY()

public:
	ABattleManager();

protected:
	virtual void BeginPlay() override;

public:
	void InitBattle();
	void StartBattle();
	void NextTurn();
	void ResetCharacters();
	bool CheckGameEnd();

	virtual void Tick(float DeltaTime) override;

	void BuildTurnOrder();
	void BuildOrderList();
	void StartNewRound();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn")
	TArray<TObjectPtr<AUBCombatUnit>> Characters;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn")
	TArray<TObjectPtr<AUBCombatUnit>> TurnOrder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn")
	TArray<TObjectPtr<AUBCombatUnit>> CurrentTurnOrder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn")
	TArray<TObjectPtr<AUBCombatUnit>> TotalTurnOrder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn")
	TObjectPtr<AUBCombatUnit> CurrentCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class UUBCharacterPortraitList> TurnOrderListWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	int32 CurrentTurn = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	int32 CurrentTurnIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target")
	TArray<TObjectPtr<AUBCombatUnit>> TargetList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target")
	TArray<TObjectPtr<AUBCombatUnit>> PlayerList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	int32 CurrentTargetIndex = 0;

	UFUNCTION(BlueprintCallable, Category = "Target")
	TArray<AUBCombatUnit*> GetTargetList();

	TArray<AUBCombatUnit*> GetSingleTargetPlayerList();

	UFUNCTION(BlueprintCallable, Category = "Turn")
	AUBCombatUnit* GetCurrentCharacter() { return CurrentCharacter; }

	AUBCombatUnit* GetRandomPlayer();
	const TArray<AUBCombatUnit*>& GetAllPlayer();

	FOnMonsterAttackStartedSignature OnMonsterAttackStarted;
	FOnMonsterAttackResultSignature OnMonsterAttackResult;
	FOnHitActionSignature OnHitAction;
	FOnTimeDelayEndedSignature OnTimeDelayEnded;

	EResultType CurrentResult = EResultType::Success;
	EResultType GetReactionResult() const { return CurrentResult; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn")
	bool bIsProcessingTurn = false;

	UFUNCTION()
	void HandleMonsterAttackResult(class AUBBaseMonster* InMonster, AUBCombatUnit* InTarget, EResultType InResult);
	void BroadcastAttackResult(EResultType InResult);
	void BroadcastAttackResultToSingle(AUBCombatUnit* InTarget, EResultType InResult);

	void SetReactionResult(EResultType InResult);
	void ForceSetActionState(EActionState InState);
	void DeathCharacterRemove(AUBCombatUnit* InDeathUnit);

	UFUNCTION(BlueprintCallable, Category = "Time")
	void GlobalTimeStop();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UIHideAllWidget();

	FOnUIHideSignature OnUIHide;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	bool bIsTimeDelay = true;

	UFUNCTION(BlueprintCallable, Category = "Time")
	void SetGlobalTimeReset();

	bool IsStun(AUBCombatUnit* InUnit);

	UFUNCTION(BlueprintNativeEvent, Category = "Game")
	void GameOver();
	virtual void GameOver_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Game")
	void GameClear();
	virtual void GameClear_Implementation();

	void HandleGameEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class UCameraComponent> TargetCamera;

	void TargetAttack(AUBCombatUnit* InTarget);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
	TObjectPtr<class ULevelSequence> SequenceCut;
};

