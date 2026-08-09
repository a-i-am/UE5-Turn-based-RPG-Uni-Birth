#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "Animation/Data/UBCharacterFXProfile.h"
#include "HealthComponent.generated.h"

class AUBCombatUnit;
class UUBStatsComponent;
class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE(FOnShieldDestroySignature);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNIBIRTH_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	void Init(UUBStatsComponent* InStatComponent);
	void ApplyDamage(int32 InDamageAmount, AUBCombatUnit* InTargetUnit, const FCharacterActionFXOverride* InImpactFX, FVector InImpactScale, FRotator InImpactRotator, int32 InHitCount);
	bool IsDead() const;
	void SpawnParticle(AUBCombatUnit* InTargetUnit, const FCharacterActionFXOverride* InImpactFX, FVector InImpactScale, FRotator InImpactRotator);
	void SpawnNiagara(AUBCombatUnit* InTargetUnit, const FCharacterActionFXOverride* InImpactFX, FVector InImpactScale, FRotator InImpactRotator);
	void Death(AUBCombatUnit* InTargetUnit);
	void SpawnGuardParticle(AUBCombatUnit* InTargetUnit);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<UUBStatsComponent> StatComponent;

	UPROPERTY(EditDefaultsOnly, Category = "FX|Guard")
	TObjectPtr<UNiagaraSystem> GuardParticle;

	FOnShieldDestroySignature OnShieldDestroy;
};

