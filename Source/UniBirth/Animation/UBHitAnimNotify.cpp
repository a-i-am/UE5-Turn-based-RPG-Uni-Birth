

#include "Animation/UBHitAnimNotify.h"
#include "Character/Enemy/UBBaseMonster.h"
UUBHitAnimNotify::UUBHitAnimNotify()
{

}

void UUBHitAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//AUBBaseMonster* monster = Cast<AUBBaseMonster>(MeshComp->GetOwner());
	//{
	//	if (monster == nullptr) return;

	//	monster->HitAcitonMultiCast();
	//}

}

