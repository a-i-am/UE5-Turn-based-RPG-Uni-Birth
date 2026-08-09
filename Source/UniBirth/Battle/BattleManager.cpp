#include "Battle/BattleManager.h"
#include "Battle/SampleCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Battle/System/UBPlayerController.h"
#include "Component/UBCharacterUI.h"
#include "Character/Enemy/UBBaseMonster.h"
#include "Battle/System/UBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Combat/UBCharacterPortraitList.h"
#include "UBTargetSelectManager.h"
#include "Component/UBStatsComponent.h"
#include "UI/Combat/UBBattleHUD.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Component/UBBuffComponent.h"
#include "UI/Combat/UBBattleSelectBase.h"
#include "Battle/System/UBGameStateBase.h"

ABattleManager::ABattleManager()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	TargetCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TargetCamera"));
	TargetCamera->SetupAttachment(RootComponent);
}

void ABattleManager::BeginPlay()
{
	Super::BeginPlay();
	CurrentTurn = 0;
	CurrentTurnIndex = 0;
}

void ABattleManager::InitBattle()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUBCombatUnit::StaticClass(), Actors);

	Characters.Empty();

	for (AActor* Actor : Actors)
	{
		if (AUBCombatUnit* Unit = Cast<AUBCombatUnit>(Actor))
		{
			Characters.Add(Unit);

			if (Unit->teamType == ETeamType::Enemy)
			{
				TargetList.Add(Unit);
			}
			else if (Unit->teamType == ETeamType::Ally)
			{
				PlayerList.Add(Unit);
			}
		}
	}

	CurrentTurn = 1;
	CurrentTurnIndex = 0;
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UUBCharacterPortraitList::StaticClass(), false);
	BuildTurnOrder();
	StartNewRound();
}

void ABattleManager::StartBattle()
{
	if (CurrentTurnOrder.Num() <= 0)
		return;

	AUBCombatUnit* Base = CurrentTurnOrder[CurrentTurnIndex];
	if (!Base)
		return;

	if (ASampleCharacter* PlayerUnit = Cast<ASampleCharacter>(Base))
	{
		CurrentCharacter = PlayerUnit;
		PlayerUnit->SetTurn(true);
		if (AUBGameStateBase* GS = GetWorld()->GetGameState<AUBGameStateBase>())
		{
			GS->bAllowPlayerUI = true;
		}
	}
	else if (AUBBaseMonster* MonsterUnit = Cast<AUBBaseMonster>(Base))
	{
		CurrentCharacter = MonsterUnit;
		MonsterUnit->SetTurn(true);
		if (AUBGameStateBase* GS = GetWorld()->GetGameState<AUBGameStateBase>())
		{
			GS->bAllowPlayerUI = false;
		}

		if (AUBAIController* AI = Cast<AUBAIController>(MonsterUnit->GetController()))
		{
			AI->GetBlackboardComponent()->SetValueAsBool("bIsMyTurn", true);
			AI->UpdateToTarget();
		}
	}
	BuildOrderList();
	if (AUBPlayerController* PC = Cast<AUBPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->OnPlayerTurnStarted(CurrentCharacter);
	}
}

void ABattleManager::NextTurn()
{
	if (bIsProcessingTurn)
		return;

	bIsProcessingTurn = true;
	HandleGameEnd();

	if (CurrentCharacter)
	{
		CurrentCharacter->BuffComp->AdvanceBuffTurns();
		CurrentCharacter->SetTurn(false);
	}

	CurrentTurnIndex++;
	BuildTurnOrder();

	if (CurrentTurnIndex >= CurrentTurnOrder.Num())
	{
		CurrentTurn++;
		StartNewRound();
	}

	if (CurrentTurnOrder.Num() <= 0)
		return;

	AUBCombatUnit* NextUnit = CurrentTurnOrder[CurrentTurnIndex];
	if (IsStun(NextUnit))
	{
		bIsProcessingTurn = false;
		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(
			DelayHandle,
			this,
			&ABattleManager::NextTurn,
			1.0f,
			false
		);
		return;
	}
	UUBGameInstance* GI = GetGameInstance<UUBGameInstance>();
	if (GI == nullptr)
		return;
	GI->SkillManager->OnTurnStart(NextUnit);
	NextUnit->HealMp(1);
	if (!NextUnit)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				NextTurn();
			});
		return;
	}

	float HpValue = NextUnit->statsComp->currentStats.Hp;

	if (HpValue <= 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				NextTurn();
			});
		return;
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick([this, NextUnit]()
		{
			CurrentCharacter = NextUnit;
			if (ASampleCharacter* PlayerUnit = Cast<ASampleCharacter>(NextUnit))
			{
				CurrentCharacter = PlayerUnit;
				PlayerUnit->SetTurn(true);

				if (AUBPlayerController* PC = Cast<AUBPlayerController>(GetWorld()->GetFirstPlayerController()))
				{
					if (AUBBattleHUD* PCHUD = Cast<AUBBattleHUD>(PC->GetHUD()))
					{
						PCHUD->CharacterUI->ChangeOwner(PlayerUnit);
						PCHUD->CharacterUI->ShowWidgetComp(PCHUD->CharacterUI->AttackWidget);
						if (PCHUD->CurrentWidget != nullptr) {
							PCHUD->CurrentWidget->PlayAnim();
						}
					}
				}
				return;
			}
			else if (AUBBaseMonster* MonsterUnit = Cast<AUBBaseMonster>(NextUnit))
			{
				CurrentCharacter = MonsterUnit;
				MonsterUnit->SetTurn(true);

				if (AUBAIController* AI = Cast<AUBAIController>(MonsterUnit->GetController()))
				{
					AI->GetBlackboardComponent()->SetValueAsBool("bIsMyTurn", true);
					OnUIHide.Broadcast();
					AI->UpdateToTarget();
				}
				return;
			}
		});
	bIsProcessingTurn = false;
}

void ABattleManager::ResetCharacters()
{
	Characters.Sort();
}

bool ABattleManager::CheckGameEnd()
{
	if (PlayerList.IsEmpty() || TargetList.IsEmpty())
	{
		return false;
	}
	return true;
}

void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABattleManager::BuildTurnOrder()
{
	TurnOrder.Empty();

	for (AUBCombatUnit* Unit : Characters)
	{
		if (!Unit)
			continue;

		float HpValue = Unit->statsComp->currentStats.Hp;

		if (HpValue > 0.0f)
		{
			TurnOrder.Add(Unit);
		}
	}

	TurnOrder.Sort([this](const AUBCombatUnit& A, const AUBCombatUnit& B)
		{
			float SpeedA = A.statsComp->currentStats.AttackSpeed;
			float SpeedB = B.statsComp->currentStats.AttackSpeed;
			return SpeedA > SpeedB;
		});

	BuildOrderList();
}

void ABattleManager::BuildOrderList()
{
	TotalTurnOrder.Empty();

	if (CurrentTurnOrder.Num() == 0 || TurnOrder.Num() == 0)
		return;

	const int32 MaxCount = 8;

	for (int32 i = CurrentTurnIndex; i < CurrentTurnOrder.Num(); ++i)
	{
		if (AUBCombatUnit* Unit = CurrentTurnOrder[i])
			TotalTurnOrder.Add(Unit);

		if (TotalTurnOrder.Num() >= MaxCount)
			return;
	}
	int32 TurnCount = TurnOrder.Num();

	for (int32 i = 0; TotalTurnOrder.Num() < MaxCount; ++i)
	{
		int32 Index = i % TurnCount;
		if (AUBCombatUnit* Unit = TurnOrder[Index])
			TotalTurnOrder.Add(Unit);
	}

	if (TurnOrderListWidget == nullptr)
		return;
	for (int32 i = 0; i < 8; ++i)
	{
		TurnOrderListWidget->SetPortraitType(i, TotalTurnOrder[i]->characterType);
	}
	TurnOrderListWidget->ReApplyAll();
}

void ABattleManager::StartNewRound()
{
	CurrentTurnOrder = TurnOrder;
	CurrentTurnIndex = 0;
}

TArray<AUBCombatUnit*> ABattleManager::GetTargetList()
{
	TArray<AUBCombatUnit*> TempList;

	for (AUBCombatUnit* TargetUnit : TargetList)
	{
		if (TargetUnit->statsComp->currentStats.Hp > 0)
		{
			TempList.Add(TargetUnit);
		}
	}

	TempList.Sort([this](const AUBCombatUnit& A, const AUBCombatUnit& B)
		{
			ECharacterType TypeA = A.characterType;
			ECharacterType TypeB = B.characterType;

			if (TypeA != TypeB)
			{
				return static_cast<uint8>(TypeA) > static_cast<uint8>(TypeB);
			}

			int32 HpA = A.statsComp->currentStats.Hp;
			int32 HpB = B.statsComp->currentStats.Hp;

			if (HpA != HpB)
			{
				return HpA < HpB;
			}

			int32 AtkA = A.statsComp->currentStats.Attack;
			int32 AtkB = B.statsComp->currentStats.Attack;

			if (AtkA != AtkB)
			{
				return AtkA > AtkB;
			}

			float X_A = A.GetActorLocation().X;
			float X_B = B.GetActorLocation().X;

			return X_A < X_B;
		});

	TargetList = TempList;
	return TargetList;
}

TArray<AUBCombatUnit*> ABattleManager::GetSingleTargetPlayerList()
{
	TArray<AUBCombatUnit*> VisiblePlayers;
	for (AUBCombatUnit* Unit : PlayerList)
	{
		if (Unit && !Unit->IsStealthed())
		{
			VisiblePlayers.Add(Unit);
		}
	}
	return VisiblePlayers;
}

AUBCombatUnit* ABattleManager::GetRandomPlayer()
{
	TArray<AUBCombatUnit*> Candidates = GetSingleTargetPlayerList();

	if (Candidates.Num() == 0)
	{
		return nullptr;
	}
	int32 TotalAggroPoint = 0;
	for (AUBCombatUnit* Unit : Candidates)
	{
		TotalAggroPoint += Unit->statsComp->currentStats.aggroPoint;
	}
	int32 RandomValue = FMath::RandRange(1, TotalAggroPoint);

	int32 AccAggro = 0;
	for (AUBCombatUnit* Unit : Candidates)
	{
		AccAggro += Unit->statsComp->currentStats.aggroPoint;

		if (RandomValue <= AccAggro)
		{
			return Unit;
		}
	}

	return nullptr;
}

const TArray<AUBCombatUnit*>& ABattleManager::GetAllPlayer()
{
	return PlayerList;
}

void ABattleManager::HandleMonsterAttackResult(AUBBaseMonster* InMonster, AUBCombatUnit* InTarget, EResultType InResult)
{
	if (!InMonster) return;
	if (InMonster->CurrentSkillData->target_scope == 3)
	{
		BroadcastAttackResult(InResult);
	}
	else if (InMonster->CurrentSkillData->target_scope == 1)
	{
		BroadcastAttackResultToSingle(InTarget, InResult);
	}
}

void ABattleManager::BroadcastAttackResult(EResultType InResult)
{
	for (AUBCombatUnit* PlayerUnit : PlayerList)
	{
		ASampleCharacter* CharacterUnit = Cast<ASampleCharacter>(PlayerUnit);
		if (!CharacterUnit) continue;
		CharacterUnit->HandleMonsterAttackResolved(nullptr, PlayerUnit, InResult);
	}
}

void ABattleManager::BroadcastAttackResultToSingle(AUBCombatUnit* InTarget, EResultType InResult)
{
	if (!InTarget) return;

	ASampleCharacter* CharacterUnit = Cast<ASampleCharacter>(InTarget);
	if (!CharacterUnit) return;
	CharacterUnit->HandleMonsterAttackResolved(nullptr, CharacterUnit, InResult);
}

void ABattleManager::SetReactionResult(EResultType InResult)
{
	CurrentResult = InResult;
}

void ABattleManager::ForceSetActionState(EActionState InState)
{
	for (AUBCombatUnit* PlayerUnit : PlayerList)
	{
		if (ASampleCharacter* CharacterUnit = Cast<ASampleCharacter>(PlayerUnit))
		{
			CharacterUnit->SetActionState(InState);
		}
	}
}

void ABattleManager::DeathCharacterRemove(AUBCombatUnit* InDeathUnit)
{
	if (ASampleCharacter* PlayerUnit = Cast<ASampleCharacter>(InDeathUnit))
	{
		PlayerList.Remove(PlayerUnit);
		CurrentTurnOrder.Remove(PlayerUnit);
	}
	else if (AUBBaseMonster* MonsterUnit = Cast<AUBBaseMonster>(InDeathUnit))
	{
		TargetList.Remove(MonsterUnit);
		CurrentTurnOrder.Remove(MonsterUnit);
	}
}

void ABattleManager::GlobalTimeStop()
{
	bIsTimeDelay = false;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
}

void ABattleManager::UIHideAllWidget()
{
	OnUIHide.Broadcast();
}

void ABattleManager::SetGlobalTimeReset()
{
	bIsTimeDelay = false;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	OnTimeDelayEnded.Broadcast();
}

bool ABattleManager::IsStun(AUBCombatUnit* InUnit)
{
	if (InUnit->statsComp->currentStats.CurrState.Contains(EUnitState::Stun))
	{
		InUnit->statsComp->currentStats.CurrState.Remove(EUnitState::Stun);
		InUnit->ActiveBuffEffect();
		return true;
	}
	return false;
}

void ABattleManager::GameOver_Implementation()
{
}

void ABattleManager::GameClear_Implementation()
{
}

void ABattleManager::HandleGameEnd()
{
	if (!CheckGameEnd())
	{
		if (PlayerList.IsEmpty())
		{
			GameOver();
		}
		else if (TargetList.IsEmpty())
		{
			GameClear();
		}
		return;
	}
}

void ABattleManager::TargetAttack(AUBCombatUnit* InTarget)
{
	ASampleCharacter* PlayerUnit = Cast<ASampleCharacter>(CurrentCharacter);
	if (!PlayerUnit) return;

	switch (PlayerUnit->GetCurrentActionState())
	{
	case EActionState::Attack:
		PlayerUnit->Attack(InTarget);
		break;
	case EActionState::ActiveSkill:
		PlayerUnit->ActiveSkill(InTarget);
		break;
	case EActionState::UltimateSkill:
		PlayerUnit->UltiMateKSill(InTarget);
		break;
	}
}




