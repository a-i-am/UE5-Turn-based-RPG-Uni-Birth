// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Decorator/UBResetLocationNotifyState.h"
#include "Battle/SampleCharacter.h"
void UUBResetLocationNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp == nullptr)return;
	character = Cast<ASampleCharacter>(MeshComp->GetOwner());
	if (character == nullptr)return;
}

void UUBResetLocationNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (character == nullptr) return;
	const FVector CurrPelvisWS = MeshComp->GetSocketLocation(PelvisBoneName);
	character->CharacterResetLocation(CurrPelvisWS, FrameDeltaTime);
}

void UUBResetLocationNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
}
