// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Decorator/UBBTCanSkillDecorator.h"
#include "Character/Interface/UBCombatUnit.h"
#include "Character/Enemy/UBBaseMonster.h"
#include "Character/Data/UBCharacterSkillData.h"
#include "Battle/System/UBAIController.h"
#include "Component/UBStatsComponent.h"
#include "Battle/System/UBGameStateBase.h"
UUBBTCanSkillDecorator::UUBBTCanSkillDecorator()
{
	NodeName = "Can use Skill";
}
bool UUBBTCanSkillDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return false;

	AUBBaseMonster* Monster = Cast<AUBBaseMonster>(AI->GetPawn());
	if (!Monster || !Monster->statsComp) return false;

	return Monster->statsComp->currentStats.Mp >= RequiredMP;

}

UUBBTCheckGameStart::UUBBTCheckGameStart()
{
	NodeName = "Check Game Start";
}

bool UUBBTCheckGameStart::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UWorld* World = OwnerComp.GetWorld();
	if (!World) return false;

	AUBGameStateBase* GS = World->GetGameState<AUBGameStateBase>();
	if (!GS)
	{
		return false;
	}
	return GS->bIsGameStart;
}
