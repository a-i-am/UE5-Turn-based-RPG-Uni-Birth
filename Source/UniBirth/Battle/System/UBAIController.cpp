// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/System/UBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Battle/BattleManager.h"
#include "Battle/System/BattleGameMode.h"
#include "Character/Interface/UBCombatUnit.h"
#include "Character/Enemy/UBBaseMonster.h"

AUBAIController::AUBAIController()
{
	ownerBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("blackboardComponent"));
	behaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTTreeComponnent"));
}

void AUBAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	if (!ownerBlackboard)	return;
	if (AUBBaseMonster* Monster = Cast<AUBBaseMonster>(pawn))
	{
		RunBehaviorTree(Monster->BehaviorTree);
	}
	ownerBlackboard->SetValueAsBool(TEXT("bIsMyTurn"), false);
	ownerBlackboard->SetValueAsBool(TEXT("bIsShieldDestroy"), false);
	ownerBlackboard->SetValueAsBool(TEXT("bIsBrokenShield"), true);
}
void AUBAIController::UpdateToTarget()
{
	if (ABattleGameMode* GM = GetWorld()->GetAuthGameMode<ABattleGameMode>())
	{
		if (ABattleManager* BM = GM->GetBattleManager())
		{
			target = BM->GetRandomPlayer();
			if (target == nullptr)
			{
				BM->NextTurn();
				return;
			}
			ownerBlackboard->SetValueAsObject("Target", target);
			FocusCameraOnTarget(target);
		}
	}

}

void AUBAIController::SetCheckHandle()
{
	ownerBlackboard->SetValueAsBool(TEXT("bIsBrokenShield"), false);
	ownerBlackboard->SetValueAsBool(TEXT("bIsShieldDestroy"), true);
}

void AUBAIController::FocusCameraOnTarget(AUBCombatUnit* Target)
{
	if (!Target) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetViewTargetWithBlend(Target, 0.25f);
	}
}