// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UBMotageEndAnimNotify.h"
#include "Character/Interface/UBCombatUnit.h"
#include "Character/Enemy/UBBaseMonster.h"
#include "Battle/SampleCharacter.h"
UUBMotageEndAnimNotify::UUBMotageEndAnimNotify()
{

}
//  ********  몽타주 끝났음을 알리는 노티파이 *******  (몬스터만) 
void UUBMotageEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AUBBaseMonster* Monster = Cast<AUBBaseMonster>(MeshComp->GetOwner());
	if (Monster)
	{ 
		Monster->MonsterOnActionFinished();
	}

	// 플레이어 반격기 노티파이 
	//else if (ASampleCharacter* player = Cast<ASampleCharacter>(MeshComp->GetOwner()))
	//{
	//	player->Counter();
	//}


}
