// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleManager.h"
#include "Battle/SampleCharacter.h"
#include "kismet/GameplayStatics.h"
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
#include "Battle/System/UBGameStateBase.h"

// Sets default values
ABattleManager::ABattleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent =
		CreateDefaultSubobject<USceneComponent>(TEXT("Root"));


	TargetCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TargetCamera"));
	TargetCamera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABattleManager::BeginPlay()
{
	Super::BeginPlay();
	currentTurn = 0;
	currentTurnIndex = 0;	

}

void ABattleManager::InitBattle()
{
	TArray<AActor*> acters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUBCombatUnit::StaticClass(), acters);

	characters.Empty();

	for (AActor* Actor : acters)
	{
		if (AUBCombatUnit* Unit = Cast<AUBCombatUnit>(Actor))
		{
			characters.Add(Unit);
		
			//몬스터 전용배열 
			if (Unit->teamType == ETeamType::Enemy_Team)
			{
				targetList.Add(Unit);
			}
			// 플레이어 전용배열
			else if (Unit->teamType == ETeamType::Ally_Team)
			{
				PlayerList.Add(Unit);
			}
		}
	
	}

	currentTurn = 1;
	currentTurnIndex = 0;
	TArray<UUserWidget*> widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), widgets, UUBCharacterPortraitList::StaticClass(),false);	
	BuildTurnOrder();      // TurnOrder 생성
	StartNewRound();       // CurrentTurnOrder 초기화 + 첫 턴 시작 준비
	
	//StartBattle();   

}


void ABattleManager::StartBattle()
{
	if (CurrentTurnOrder.Num() <= 0)
		return;

	AUBCombatUnit* Base = CurrentTurnOrder[currentTurnIndex];
	if (!Base)
		return;

	if (ASampleCharacter* player = Cast<ASampleCharacter>(Base))
	{
		currentCharacter = player;
		player->SetTurn(true); 
		if (AUBGameStateBase* GS = GetWorld()->GetGameState<AUBGameStateBase>())
		{
			GS->bAllowPlayerUI = true;
		}

		if(AUBPlayerController* PC = Cast<AUBPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("PC->onPlayerTurnStart"));
			//PC->OnPlayerTurnStarted(currentCharacter);
		}
	}
	else if (AUBBaseMonster* monster = Cast<AUBBaseMonster>(Base))
	{
		currentCharacter = monster;
		monster->SetTurn(true);
		if (AUBGameStateBase* GS = GetWorld()->GetGameState<AUBGameStateBase>())
		{
			GS->bAllowPlayerUI = false;
		}

		if (AUBAIController* AI = Cast<AUBAIController>(monster->GetController()))
		{
			AI->GetBlackboardComponent()->SetValueAsBool("bIsMyTurn", true);
			AI->UpdateToTarget();

		}
	}
	BuildOrderList();
	if (AUBPlayerController* PC = Cast<AUBPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->OnPlayerTurnStarted(currentCharacter);
	}
}
	
void ABattleManager::NextTurn()
{
	if (bIsProcessingTurn)
		return;
	

	bIsProcessingTurn = true;
	HandleGameEnd();

	
	if (currentCharacter)
	{
		currentCharacter->BuffComp->AdvanceBuffTurns();

		currentCharacter->SetTurn(false);
	}
	
	currentTurnIndex++;
	BuildTurnOrder();
	
	if (currentTurnIndex >= CurrentTurnOrder.Num())
	{
		currentTurn++;
		StartNewRound();
	}
	
	if (CurrentTurnOrder.Num() <= 0)
		return;
	
	AUBCombatUnit* NextUnit = CurrentTurnOrder[currentTurnIndex];
	if (bIsStun(NextUnit))
	{
		AUBPlayerController* PC = Cast<AUBPlayerController>(GetWorld()->GetFirstPlayerController());
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

	float HpValue = 0.0f;
	HpValue = NextUnit->statsComp->currentStats.Hp;

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
			currentCharacter = NextUnit;
			if (ASampleCharacter* player = Cast<ASampleCharacter>(NextUnit))
			{
				currentCharacter = player;
				player->SetTurn(true);
			
				if (AUBPlayerController* PC = Cast<AUBPlayerController>(GetWorld()->GetFirstPlayerController()))
				{
					if (AUBBattleHUD* PCHUD = Cast<AUBBattleHUD>(PC->GetHUD())) 
					{
						PCHUD->CharacterUI->ChangeOwner(player);
						PCHUD->CharacterUI->ShowWidgetComp(PCHUD->CharacterUI->AttackWidget);
						if (PCHUD->CurrentWidget != nullptr) {
							PCHUD->CurrentWidget->PlayAnim();
						}
					}
				}
				return;
			}
			else if (AUBBaseMonster* monster = Cast<AUBBaseMonster>(NextUnit))
			{
				currentCharacter = monster;
				monster->SetTurn(true);

				if (AUBAIController* AI = Cast<AUBAIController>(monster->GetController()))
				{
					AI->GetBlackboardComponent()->SetValueAsBool("bIsMyTurn", true);
					OnUIHide.Broadcast();
					AI->UpdateToTarget();

				}
				return;
			}
		});
	bIsProcessingTurn = false;
	//NextTurn();
}


void ABattleManager::ResetCharacters()
{
	//스피드에 따라서 정렬하기 나중에 함수로 빼야할듯(혹은 캐릭터에 오퍼레이터 넣거나)
	// 매 턴이 지날때마다 실행해야함
	/*characters.Sort([](const ASampleCharacter& A, const ASampleCharacter& B) {
		return A.speed > B.speed;
		});*/
	characters.Sort();
}

//게임이 끝났는지 확인
bool ABattleManager::CheckGameEnd()
{
	if (PlayerList.IsEmpty()|| targetList.IsEmpty())
	{
		return false;
	}
	else {
		return true;
	}
}


// Called every frame
void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABattleManager::BuildTurnOrder()
{
	TurnOrder.Empty();


	for (AUBCombatUnit* Unit : characters)
	{
		if (!Unit)
			continue;

		float HpValue = 0.0f;
		HpValue = Unit->statsComp->currentStats.Hp;

		if (HpValue > 0.0f)
		{
			TurnOrder.Add(Unit);
		}
	}

	// 속도 기준 정렬 (빠른 순으로)
	TurnOrder.Sort([this](const AUBCombatUnit& A, const AUBCombatUnit& B)
		{
			float SpeedA = A.statsComp->currentStats.AttackSpeed;
			float SpeedB = B.statsComp->currentStats.AttackSpeed;
			return SpeedA > SpeedB; // 빠른 순
		});

	BuildOrderList();
}

void ABattleManager::BuildOrderList()
{
	TotalTurnOrder.Empty();

	if (CurrentTurnOrder.Num() == 0 || TurnOrder.Num() == 0)
		return;

	const int32 MaxCount = 8;

	for (int32 i = currentTurnIndex; i < CurrentTurnOrder.Num(); ++i)
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

	if (turnOrderListWidget == nullptr)
		return;
	for (int32 i =0; i<8; ++i)
	{
		turnOrderListWidget->SetPortraitType(i, TotalTurnOrder[i]->characterType);
	}
	turnOrderListWidget->ReApplyAll();
}

void ABattleManager::StartNewRound()
{
	// TurnOrder 내용을 그대로 복사해서,
	// 이 라운드 동안만 사용할 CurrentTurnOrder 생성
	CurrentTurnOrder = TurnOrder;
	currentTurnIndex = 0;
}

TArray<AUBCombatUnit*> ABattleManager::GetTargetList()
{
	TArray<AUBCombatUnit*> templist;

	for (AUBCombatUnit* target : targetList)
	{
		if (target->statsComp->currentStats.Hp > 0)
		{
			templist.Add(target);
		}
	}

	templist.Sort([this](const AUBCombatUnit& A, const AUBCombatUnit& B)
		{
			//
			// 1) 캐릭터 타입 비교 (enum → 높은 타입 우선)
			//
			ECharacterType TypeA = A.characterType;
			ECharacterType TypeB = B.characterType;

			if (TypeA != TypeB)
			{
				return static_cast<uint8>(TypeA) > static_cast<uint8>(TypeB);
			}

			//
			// 2) HP 비교 (낮은 HP 우선)
			//
			int HpA = A.statsComp->currentStats.Hp;
			int HpB = B.statsComp->currentStats.Hp;

			if (HpA != HpB)
			{
				return HpA < HpB; // 적은 체력 먼저
			}

			//
			// 3) 공격력 비교 (높은 ATK 우선)
			//
			int AtkA = A.statsComp->currentStats.Attack;
			int AtkB = B.statsComp->currentStats.Attack;

			if (AtkA != AtkB)
			{
				return AtkA > AtkB;
			}

			//
			// 4) 왼쪽에 있는 적 우선 (X 좌표)
			//
			float X_A = A.GetActorLocation().X;
			float X_B = B.GetActorLocation().X;

			return X_A < X_B;
		});

	targetList = templist;

	return targetList;
}
// 단일 타겟가능한 대상만 추리기
TArray<AUBCombatUnit*> ABattleManager::GetSingleTargetPlayerList()
{
	TArray<AUBCombatUnit*> VisiblePlayers;
	for (AUBCombatUnit* Unit : PlayerList)
	{
		if (Unit && !Unit->IsStealthed())   // 은신 제외
		{
			VisiblePlayers.Add(Unit);
		}
	}
	return VisiblePlayers;
}

//랜덤한 플레이어 갖고오기
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
	int32 randomValue = FMath::RandRange(1, TotalAggroPoint);

	int32 AccAggro = 0;
	for (AUBCombatUnit* Unit : Candidates)
	{
		AccAggro += Unit->statsComp->currentStats.aggroPoint;

		if (randomValue <= AccAggro)
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

void ABattleManager::HandleMonsterAttackResult(AUBBaseMonster* Monster, AUBCombatUnit* Target, EResultType Result)
{
	if (!Monster) return;
	if (Monster->CurrentSkillData->target_scope == 3)
	{
		BrodacastAttackResult(Result);
	}
	else if (Monster->CurrentSkillData->target_scope == 1)
	{
		BroadcastAttackResultToSingle(Target, Result);
	}
}

void ABattleManager::BrodacastAttackResult(EResultType Result)
{
	
	for (AUBCombatUnit* ps : PlayerList)
	{
		ASampleCharacter* character = Cast<ASampleCharacter>(ps);
		if (!character) continue;
		character->HandleMonsterAttackResolved(nullptr, ps, Result);
	}
}

void ABattleManager::BroadcastAttackResultToSingle(AUBCombatUnit* Target, EResultType Result)
{
	if (!Target) return;
	
	ASampleCharacter* character = Cast<ASampleCharacter>(Target);
	if (!character) return;
	character->HandleMonsterAttackResolved(nullptr, character, Result);
}

void ABattleManager::SetReactionResult(EResultType InResult)
{
	CurrentResult = InResult;
}

//전체스테이트 변환
void ABattleManager::ForceSetActionState(EActionState State)
{
	for (AUBCombatUnit* ps : PlayerList)
	{
		if (ASampleCharacter* ch = Cast<ASampleCharacter>(ps))
		{
			ch->SetActionState(State);
		}
	}
}

void ABattleManager::DeathCharacterReMove(AUBCombatUnit* deathUnit)
{
	if (ASampleCharacter* player = Cast<ASampleCharacter>(deathUnit))
	{
		PlayerList.Remove(player);
		CurrentTurnOrder.Remove(player);
	}
	else if(AUBBaseMonster* monster = Cast<AUBBaseMonster>(deathUnit))
	{
		targetList.Remove(monster);
		CurrentTurnOrder.Remove(monster);
	}



}

void ABattleManager::GlobalTimeStop()
{
	bIsTimeDelay = false;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
}

void ABattleManager::UIHideAllWiget()
{
	OnUIHide.Broadcast();
}

void ABattleManager::SetGlobalTimeReset()
{
	bIsTimeDelay = false;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	OnTimeDelayEnded.Broadcast();
}

bool ABattleManager::bIsStun(AUBCombatUnit* unit)
{
	if (unit->statsComp->currentStats.CurrState.Contains(EUnitState::Stun))
	{
		unit->statsComp->currentStats.CurrState.Remove(EUnitState::Stun);
		unit->ActiveBuffEffect();
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
		if (PlayerList.IsEmpty()) {

			GameOver();
		}
		else if (targetList.IsEmpty()) {
			GameClear();

		}
		return;
	}
}

void ABattleManager::TargetAttack(AUBCombatUnit* target)
{
	ASampleCharacter* player = Cast<ASampleCharacter>(currentCharacter);
	switch (player->GetCurrentActionState())
	{
	case EActionState::Attack:
		player->Attack(target);
		break;
	case EActionState::ActiveSkill:
		player->ActiveSkill(target);
		break;
	case EActionState::UltiMateSkill:
		player->UltiMateKSill(target);
		break;
	}
	
}



