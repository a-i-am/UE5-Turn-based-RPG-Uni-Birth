#include "Component/HealthComponent.h"
#include "Animation/Data/UBCharacterFXProfile.h"
#include "Kismet/GameplayStatics.h"
#include  "Component/UBStatsComponent.h"
#include "Character/Interface/UBCombatUnit.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UHealthComponent::Init(UUBStatsComponent* inStatComp)
{
}

void UHealthComponent::ApplyDamage(int32 DamageAmount, AUBCombatUnit* target,
	const FCharacterActionFXOverride* ImpactFX, FVector ImpactScale, FRotator ImpactRotator, int32 hitCount)
{
	if (!target || hitCount <= 0)
		return;
	//히트당 데미지 계산
	int32 DamagePerHit = DamageAmount / hitCount;

	//데미지 텍스트 출력
	target->PrintDamage(DamagePerHit);
	
	 // 쉴드 @TODO
	int32& Shield = target->statsComp->currentStats.shield;
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
			target->HealthComp->OnShieldDestroy.Broadcast();
		}
	}

 	target->statsComp->currentStats.Hp -= DamagePerHit;
	
	if (ImpactFX)
	{
		if (ImpactFX->Particle)
		{
			SpawnParticle(target, ImpactFX, ImpactScale, ImpactRotator);
		}
		else
		{
			SpawnNiagara(target, ImpactFX, ImpactScale, ImpactRotator);
		}
	}
	if (target->bIsDead()) return;

	if (target->statsComp->currentStats.Hp <= 0)
	{
		target->statsComp->currentStats.Hp = 0;
		Death(target);
		return;
	}
}

bool UHealthComponent::bIsDead() const
{
	return statComp->currentStats.Hp <= 0;
}

void UHealthComponent::SpawnParticle(AUBCombatUnit* target, const FCharacterActionFXOverride* ImpactFX, FVector ImpactScale, FRotator ImpactRotator)
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Black, TEXT(" SPAWN PARTICEL"));
	(UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		ImpactFX->Particle,
		target->GetActorLocation(),	
		ImpactRotator,
		ImpactScale));
}

void UHealthComponent::SpawnNiagara(AUBCombatUnit* target,const FCharacterActionFXOverride* ImpactFX,
	FVector ImpactScale, FRotator ImpactRotator)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		ImpactFX->NiagaraFX,
		target->GetActorLocation(),
		ImpactRotator,
		ImpactScale,
		true,
		true
	);
}

void UHealthComponent::Death(AUBCombatUnit* unit)
{
	unit->DeathCharacter();
}
//가드 이펙트 소폰
void UHealthComponent::SpawnGuardParticle(AUBCombatUnit* unit)
{
	if (!unit || !GuardParticle) return;

	const FVector UnitLocation = unit->GetActorLocation();
	const FVector Forward = unit->GetActorForwardVector();

	const FVector SpawnLocation = UnitLocation + (Forward * 30.f);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		unit->GetWorld(),
		GuardParticle,
		SpawnLocation,
		unit->GetActorRotation(),
		FVector(1.f)
	);
}


