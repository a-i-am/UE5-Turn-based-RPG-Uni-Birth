// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UBPreAttackIndicatorLocation.h"
#include "Battle/SampleCharacter.h"
#include "Niagara/Classes/NiagaraSystem.h"
#include "Character/Enemy/UBBaseMonster.h"
#include "Kismet/GameplayStatics.h"
#include "Component/HealthComponent.h"
void UUBPreAttackIndicatorLocation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AUBBaseMonster* Monster = Cast<AUBBaseMonster>(MeshComp->GetOwner());
	if (Monster == nullptr) return;

	UWorld* World = Monster->GetWorld();
	if (!World) return;

	for (AUBCombatUnit* Player : Monster->ResolvedTargets)
	{

		if (FX)
		{
			FVector location = Player->GetActorLocation();
			location.Z -= 90;
			UGameplayStatics::SpawnEmitterAtLocation(
				World,
				FX,
				location,
				FXRotation,
				FXScale
			);
		}
		else
		{
			FVector location = Player->GetActorLocation();
			location += FXLocation;
			UNiagaraComponent* NiagaraComp =
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					World,
					NiagaraParticle,
					location,
					FXRotation,
					FXScale);
		}
	}

}

