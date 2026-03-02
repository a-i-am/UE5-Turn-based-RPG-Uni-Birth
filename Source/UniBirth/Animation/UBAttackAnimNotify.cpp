// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UBAttackAnimNotify.h"
#include "Character/Interface/UBCombatUnit.h"

//   ******실제   공격 노티파이  ***********
void UUBAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AUBCombatUnit* unit = Cast<AUBCombatUnit>(MeshComp->GetOwner());
	if (unit)
	{
		unit->AttackHit();
	}
}
