// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UBWeaponRotationAnimNotify.h"
#include "Battle/SampleCharacter.h"
void UUBWeaponRotationAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AUBPlayer* player = Cast<AUBPlayer>(MeshComp->GetOwner());
	if (player)
	{
		player->TOggleWeaponRotator();
		
	}
}
