// Fill out your copyright notice in the Description page of Project Settings.

#include "Battle/System/MonsterNode/UBTService_IncreaseAttackPerTurn.h"
#include "AIController.h"
#include "Component/UBStatsComponent.h"
#include "Character/Interface/UBCombatUnit.h"

UUBTService_IncreaseAttackPerTurn::UUBTService_IncreaseAttackPerTurn()
{
    NodeName = "Increase Attack 10%";
}

void UUBTService_IncreaseAttackPerTurn::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if (bAlreadyIncreased) return;

    AUBCombatUnit* Unit = Cast<AUBCombatUnit>(OwnerComp.GetAIOwner()->GetPawn());
    if (Unit && Unit->statsComp)
    {
        Unit->statsComp->currentStats.Attack *= 1.1f;  // 10% Áõ°¡

        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow,
            FString::Printf(TEXT("Attack +10%%: %.1f"), Unit->statsComp->currentStats.Attack));

        bAlreadyIncreased = true;
    }
}