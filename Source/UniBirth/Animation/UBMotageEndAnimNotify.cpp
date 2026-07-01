// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UBMotageEndAnimNotify.h"
#include "Character/Interface/UBCombatUnit.h"
#include "Character/Enemy/UBBaseMonster.h"
#include "Battle/SampleCharacter.h"
UUBMotageEndAnimNotify::UUBMotageEndAnimNotify()
{

}

void UUBMotageEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AUBBaseMonster* Monster = Cast<AUBBaseMonster>(MeshComp->GetOwner());
	if (Monster)
	{
		Monster->MonsterOnActionFinished();
	}








}
