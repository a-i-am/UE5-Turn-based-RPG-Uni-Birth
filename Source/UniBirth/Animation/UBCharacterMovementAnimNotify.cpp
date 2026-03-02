// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UBCharacterMovementAnimNotify.h"
#include "Battle/SampleCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Battle/BattleManager.h"
#include "Battle/System/BattleGameMode.h"
void UUBCharacterMovementAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp == nullptr)return;
	character = Cast<ASampleCharacter>(MeshComp->GetOwner());
	if (character == nullptr)return;

}


void UUBCharacterMovementAnimNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (character == nullptr) return;
	
	// 실제 좌표 이동하는 로직
	const FVector CurrPelvisWS = MeshComp->GetSocketLocation(PelvisBoneName);
	character->MoveTowardTargetFromSocket(CurrPelvisWS, character->CurrentTarget, FrameDeltaTime);
}

void UUBCharacterMovementAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
}
