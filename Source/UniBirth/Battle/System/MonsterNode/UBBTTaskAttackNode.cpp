// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/System/MonsterNode/UBBTTaskAttackNode.h"
#include "Battle/System/UBAIController.h"
#include "Character/Enemy/UBBaseMonster.h"
#include "Character/UBPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "GameplayTags/UBGameplayTags.h
UUBBTTaskAttackNode::UUBBTTaskAttackNode()
{
    NodeName = "Attack";
}

EBTNodeResult::Type UUBBTTaskAttackNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AUBAIController* AI = Cast<AUBAIController>(OwnerComp.GetAIOwner());
    if (!AI) return EBTNodeResult::Failed;
  
    AUBBaseMonster* monster = Cast<AUBBaseMonster>(AI->GetPawn());
    if (!monster)return EBTNodeResult::Failed;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

    if (AgroTarget)
    {
        //@TODO 어그로 수치에대한처리

    }
    //타겟 갖고오는로직
    AUBCombatUnit* target = Cast<AUBCombatUnit>(BB->GetValueAsObject("Target"));
 
    if (!target) return EBTNodeResult::Failed;

    monster->StartAttackSequence(lastActionTag, target);
    monster->lastSkillList.Add(lastActionTag);
    monster->OnActionFinishedDelegate.AddUObject(
        this,
        &UUBBTTaskAttackNode::OnActionFinished,
        &OwnerComp
    );
    return EBTNodeResult::InProgress;

}
void UUBBTTaskAttackNode::OnActionFinished(UBehaviorTreeComponent* OwnerComp)
{
    if (!OwnerComp) return;
    FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
