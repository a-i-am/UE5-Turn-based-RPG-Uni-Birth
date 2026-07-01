// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GamePlayTags/UBGameplayTags.h"
#include "Animation/Data/UBCharacterFXProfile.h"
#include "HealthComponent.generated.h"
class AUBCombatUnit;
class UUBStatsComponent;
DECLARE_MULTICAST_DELEGATE(FOnShieldDestroy)
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNIBIRTH_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHealthComponent();

	void Init(UUBStatsComponent* inStatComp);
	void ApplyDamage(int32 damage, class AUBCombatUnit* target, const FCharacterActionFXOverride* ImpactFX, FVector ImpactScale,
		FRotator ImpactRotator, int32 hitCount);
	bool bIsDead() const;
	void SpawnParticle(AUBCombatUnit* target, const FCharacterActionFXOverride* ImpactFX, FVector ImpactScale, FRotator ImpactRotator);
	void SpawnNiagara(AUBCombatUnit* target, const FCharacterActionFXOverride* ImpactFX, FVector ImpactScale, FRotator ImpactRotator);
	void Death(AUBCombatUnit* unit);
	void SpawnGuardParticle(AUBCombatUnit* unit);

protected:

	virtual void BeginPlay() override;



public:
	UPROPERTY()
	UUBStatsComponent* statComp;

	UPROPERTY(EditDefaultsOnly, Category = "FX|Guard")
	TObjectPtr<UNiagaraSystem> GuardParticle;

	FOnShieldDestroy OnShieldDestroy;

};
