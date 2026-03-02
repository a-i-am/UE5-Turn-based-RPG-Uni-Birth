// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UBAttackStartAnimNotify.h"
#include "GameFramework/Actor.h"
#include "Character/UBPlayer.h"
#include "Character/Enemy/UBBaseMonster.h"
#include "Battle/BattleManager.h"
#include "Battle/System/BattleGameMode.h"
#include "Character/Interface/UBCombatUnit.h"

// *********  UI 띄우게하는 노티파이  ***********
void UUBAttackStartAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;
	AUBBaseMonster* Monster = Cast<AUBBaseMonster>(MeshComp->GetOwner());
	if (!Monster) return;
	Monster->NotifyAttackIntent();

}





