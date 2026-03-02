// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/System/MonsterNode/UBChangePhaseTaskNode.h"
#include "Character/Enemy/UBBaseMonster.h"
#include "Battle/System/UBAIController.h"
#include "Component/UBStatsComponent.h"
#include "Battle/System/UBPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Combat/UBBattleHUD.h"
UUBChangePhaseTaskNode::UUBChangePhaseTaskNode()
{
	NodeName = "TreeChange Task";
}	

EBTNodeResult::Type UUBChangePhaseTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AUBAIController* AI = Cast<AUBAIController>(OwnerComp.GetAIOwner());
	if (!AI) return EBTNodeResult::Failed;

	AUBBaseMonster* Monster = Cast<AUBBaseMonster>(AI->GetPawn());
	if (!Monster) return EBTNodeResult::Succeeded;
	// 쉴드안깨짐 리턴
	if (Monster->bIsPlay == false)
	{
		return EBTNodeResult::Failed;
	}
	//카메라 캐시
	AUBPlayerController* PC = Cast<AUBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		CachedViewTarget = PC->GetViewTarget();
	}                
	//안전하게 바인딩
	Monster->OnCutSceneFinished.RemoveAll(this);
	Monster->OnCutSceneFinished.AddUObject(
		this,
		&UUBChangePhaseTaskNode::HandleCutsceneFinished,
		&OwnerComp
	);
	
	
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("CALL"));
	//쉴드 깨졌으면 
	AI->GetBrainComponent()->PauseLogic(TEXT("PhaseCutscene"));

	AI->SetCheckHandle();
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT(" true change"));


	Monster->bIsPlay = false;
	Monster->SheildRefill();
	Monster->PlayCutscene(Sequence);

	return EBTNodeResult::InProgress;


}

void UUBChangePhaseTaskNode::HandleCutsceneFinished(UBehaviorTreeComponent* OwnerComp)
{
	//비동기 Task
	FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
	AUBAIController* AI = Cast<AUBAIController>(OwnerComp->GetAIOwner());
	if (AI)
	{
		AI->GetBrainComponent()->ResumeLogic(TEXT("PhaseCutscene"));
	}
	AUBPlayerController* PC = Cast<AUBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && CachedViewTarget)
	{

		PC->SetViewTargetWithBlend(CachedViewTarget, 0.3f);
		CachedViewTarget = nullptr;
		if (AUBBattleHUD* PCHUD = Cast<AUBBattleHUD>(PC->GetHUD()))
		{
			PCHUD->bIsAngry = true;
		}

	}
}


