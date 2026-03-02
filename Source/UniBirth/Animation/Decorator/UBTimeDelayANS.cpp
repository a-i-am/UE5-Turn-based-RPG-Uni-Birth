// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Decorator/UBTimeDelayANS.h"
#include "Battle/System/BattleGameMode.h"
#include "Battle/BattleManager.h"
#include "Kismet/GameplayStatics.h"
void UUBTimeDelayANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!MeshComp) 
		return;                  
	World = MeshComp->GetWorld();   
	if (!World)
		return;                     

	ABattleGameMode* GM = World->GetAuthGameMode<ABattleGameMode>(); 
	if (!GM)
	return;
	BM = GM->GetBattleManager();
	BM->CurrentResult = EResultType::None;
	if (BM->bIsTimeDelay == false)
	{
		BM->bIsTimeDelay = true;
	}
}

void UUBTimeDelayANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (BM == nullptr) return;
	if (BM->bIsTimeDelay)
	{
		UGameplayStatics::SetGlobalTimeDilation(World, 0.15f);
	}
}

void UUBTimeDelayANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (BM == nullptr) return;
	if (BM->CurrentResult == EResultType::None)
	{
		BM->CurrentResult = EResultType::Fail;
	}
	if(BM->bIsTimeDelay)
	BM->SetGlobalTimeReset();

	

}
