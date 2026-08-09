#include "Component/HealthComponent.h"
#include "Animation/Data/UBCharacterFXProfile.h"
#include "Kismet/GameplayStatics.h"
#include "Component/UBStatsComponent.h"
#include "Character/Interface/UBCombatUnit.h"
#include "NiagaraFunctionLibrary.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::Init(UUBStatsComponent* InStatComponent)
{
	StatComponent = InStatComponent;
}

void UHealthComponent::ApplyDamage(int32 InDamageAmount, AUBCombatUnit* InTargetUnit,
	const FCharacterActionFXOverride* InImpactFX, FVector InImpactScale, FRotator InImpactRotator, int32 InHitCount)
{
	if (!InTargetUnit || InHitCount <= 0)
		return;

	int32 DamagePerHit = InDamageAmount / InHitCount;

	InTargetUnit->PrintDamage(DamagePerHit);

	int32& Shield = InTargetUnit->statsComp->currentStats.shield;
	if (Shield > 0)
	{
		if (Shield >= DamagePerHit)
		{
			Shield -= DamagePerHit;
			DamagePerHit = 0;
		}
		else
		{
			DamagePerHit -= Shield;
			Shield = 0;
			InTargetUnit->HealthComp->OnShieldDestroy.Broadcast();
		}
	}

	InTargetUnit->statsComp->currentStats.Hp -= DamagePerHit;

	if (InImpactFX)
	{
		if (InImpactFX->Particle)
		{
			SpawnParticle(InTargetUnit, InImpactFX, InImpactScale, InImpactRotator);
		}
		else
		{
			SpawnNiagara(InTargetUnit, InImpactFX, InImpactScale, InImpactRotator);
		}
	}
	if (InTargetUnit->bIsDead()) return;

	if (InTargetUnit->statsComp->currentStats.Hp <= 0)
	{
		InTargetUnit->statsComp->currentStats.Hp = 0;
		Death(InTargetUnit);
		return;
	}
}

bool UHealthComponent::IsDead() const
{
	if (!StatComponent) return false;
	return StatComponent->currentStats.Hp <= 0;
}

void UHealthComponent::SpawnParticle(AUBCombatUnit* InTargetUnit, const FCharacterActionFXOverride* InImpactFX, FVector InImpactScale, FRotator InImpactRotator)
{
	if (!InTargetUnit || !InImpactFX) return;
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		InImpactFX->Particle,
		InTargetUnit->GetActorLocation(),
		InImpactRotator,
		InImpactScale);
}

void UHealthComponent::SpawnNiagara(AUBCombatUnit* InTargetUnit, const FCharacterActionFXOverride* InImpactFX,
	FVector InImpactScale, FRotator InImpactRotator)
{
	if (!InTargetUnit || !InImpactFX) return;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		InImpactFX->NiagaraFX,
		InTargetUnit->GetActorLocation(),
		InImpactRotator,
		InImpactScale,
		true,
		true
	);
}

void UHealthComponent::Death(AUBCombatUnit* InTargetUnit)
{
	if (InTargetUnit)
	{
		InTargetUnit->DeathCharacter();
	}
}

void UHealthComponent::SpawnGuardParticle(AUBCombatUnit* InTargetUnit)
{
	if (!InTargetUnit || !GuardParticle) return;

	const FVector UnitLocation = InTargetUnit->GetActorLocation();
	const FVector Forward = InTargetUnit->GetActorForwardVector();

	const FVector SpawnLocation = UnitLocation + (Forward * 30.f);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		InTargetUnit->GetWorld(),
		GuardParticle,
		SpawnLocation,
		InTargetUnit->GetActorRotation(),
		FVector(1.f)
	);
}



