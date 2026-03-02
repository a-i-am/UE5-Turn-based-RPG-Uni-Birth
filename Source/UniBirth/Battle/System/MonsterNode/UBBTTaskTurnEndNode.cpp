// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/System/MonsterNode/UBBTTaskTurnEndNode.h"
#include "Battle/System/UBAIController.h"
#include  "BehaviorTree/BTTaskNode.h"
#include  "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Battle/BattleManager.h"
#include  "Battle/System/BattleGameMode.h"
UUBBTTaskTurnEndNode::UUBBTTaskTurnEndNode()
{
	NodeName = "TurnEnd";
}
EBTNodeResult::Type UUBBTTaskTurnEndNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AUBAIController* AI = Cast<AUBAIController>(OwnerComp.GetAIOwner());
    if (!AI) return EBTNodeResult::Failed;
    AI->GetBlackboardComponent()->SetValueAsBool("bIsMyTurn", false);
    
    // ео а╬╥А
    if (ABattleGameMode* GM = GetWorld()->GetAuthGameMode<ABattleGameMode>())
    {
        if (ABattleManager* BM = GM->GetBattleManager())
        {
            BM->NextTurn();
        }
    }
    return EBTNodeResult::Succeeded;
}