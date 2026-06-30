// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Interface/UBCombatUnit.h"
#include  "Component/UBStatsComponent.h"
#include "Component/HealthComponent.h"
#include "Component/UBBuffComponent.h"
#include "Common/SubSystem/UBSkillManager.h"
#include "Common/System/UBGameInstance.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "Component/ActionSystemComponent.h"
#include "Battle/System/BattleGameMode.h"
#include "Common/SubSystem/UBDamageSubsystem.h"
#include "Components/BoxComponent.h"
#include "UI/Combat/UBDamageFloat.h"
#include "NiagaraComponent.h"

AUBCombatUnit::AUBCombatUnit()
{

	PrimaryActorTick.bCanEverTick = true;
	statsComp = CreateDefaultSubobject<UUBStatsComponent>(TEXT("StatsComp"));
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	BuffComp = CreateDefaultSubobject<UUBBuffComponent>(TEXT("BuffComp"));
	asComp = CreateDefaultSubobject<UActionSystemComponent>(TEXT("actioncomp"));



	floatingDamageArea = CreateDefaultSubobject<UBoxComponent>(TEXT("floatingDamageArea"));
	floatingDamageArea->SetupAttachment(GetRootComponent());
	floatingDamageArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	floatingDamageArea->SetGenerateOverlapEvents(false);

	BurnEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BurnEffect"));
	BurnEffect->SetupAttachment(GetRootComponent());
	BurnEffect->SetAutoActivate(false);

	StunEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("StunEffect"));
	StunEffect->SetupAttachment(GetRootComponent());
	StunEffect->SetAutoActivate(false);

	PoisoningEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PoisoningEffect"));
	PoisoningEffect->SetupAttachment(GetRootComponent());
	PoisoningEffect->SetAutoActivate(false);
}


void AUBCombatUnit::BeginPlay()
{
	Super::BeginPlay();
	if(HealthComp)
	HealthComp->Init(statsComp);



	SkillManager = GetGameInstance()->GetSubsystem<UUBSkillManager>();
	if (SkillManager == nullptr) return;

	if (gameInstance = GetGameInstance<UUBGameInstance>())
	{
		gameInstance->GetSkillsForCharacter(characterType, OwnedSkills, OwnedPassvieSkills);

	}ABattleGameMode* gm = GetWorld()->GetAuthGameMode<ABattleGameMode>();
	damageSST = UGameInstance::GetSubsystem<UUBDamageSubsystem>(GetWorld()->GetGameInstance());
	bm = gm->GetBattleManager();
	if (bm == nullptr) return;
	if(damageSST == nullptr) return;

	if (statsComp)
	{
		statsComp->InitStats(characterType);
	}

}


void AUBCombatUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AUBCombatUnit::AttackHit()
{

}


void AUBCombatUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FCharacterSkill* AUBCombatUnit::FindSkillKey(FString keyName)
{
	return OwnedSkills.FindByPredicate(
		[&](const FCharacterSkill& s)
		{
			return s.skill_key == keyName;
		});
}

FCharacterSkill* AUBCombatUnit::FindSkillId(int32 skillId)
{
	return OwnedSkills.FindByPredicate(
		[&](const FCharacterSkill& s)
		{
			return s.skill_id == skillId;
		});
}
void AUBCombatUnit::PlayCutscene(ULevelSequence* Sequence)
{
	FMovieSceneSequencePlaybackSettings Settings;
	ALevelSequenceActor* OutActor = nullptr;

	ULevelSequencePlayer* Player =
		ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(),
			Sequence,
			Settings,
			OutActor
		);
	if (Player == nullptr)
	{
		return;
	}


	cachedSequenceActor = OutActor;
	Player->OnFinished.AddDynamic(this, &AUBCombatUnit::OnSequenceFinished);

	Player->Play();
}
void AUBCombatUnit::OnSequenceFinished()
{
	if (IsValid(cachedSequenceActor))
	{
		cachedSequenceActor->Destroy();
		cachedSequenceActor = nullptr;

	}
	OnCutSceneFinished.Broadcast();
}

void AUBCombatUnit::DeathCharacter()
{

}



bool AUBCombatUnit::bIsDead()
{
	return bIsDeath;
}

void AUBCombatUnit::HealMp(const int value)
{

	if (statsComp->currentStats.Mp > 10)
	{
		return;
	}

	statsComp->currentStats.Mp += value;
	if (statsComp->currentStats.Mp > 10)
	{
		statsComp->currentStats.Mp = 10;
	}
}

bool AUBCombatUnit::IsStealthed()
{
	return statsComp->currentStats.CurrState.Contains(EUnitState::Stealth);
}

void AUBCombatUnit::Counter(AUBCombatUnit* unit)
{

}

void AUBCombatUnit::PrintDamage(int damage)
{

	const FVector Extent = floatingDamageArea->GetScaledBoxExtent();
	const FVector Origin = floatingDamageArea->GetComponentLocation();

	FVector pos = Origin + FVector(
		FMath::FRandRange(-Extent.X, Extent.X),
		FMath::FRandRange(-Extent.Y, Extent.Y),
		FMath::FRandRange(-Extent.Z, Extent.Z));

	if (result == EResultType::Success)
	{
		switch (currentState)
		{
		case EActionState::Dodge:
			pos = Origin;
			break;
		case EActionState::Parry:
			pos = Origin;
			break;

		}
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AUBDamageFloat* DamageFloat =
		GetWorld()->SpawnActor<AUBDamageFloat>(
			damageFloatUI,
			pos,
			FRotator::ZeroRotator,
			Params
		);
	if (DamageFloat)
	{
		DamageFloat->SetDamageText(damage);
		if (result == EResultType::Success)
		{
			DamageFloat->SetDefenceAction(currentState);
		}
		else {
			DamageFloat->SetDefenceAction(EActionState::None);
		}
	}
}

void AUBCombatUnit::ActiveBuffEffect()
{
	if (!statsComp)
		return;

	const TArray<EUnitState>& States = statsComp->currentStats.CurrState;


	if (BurnEffect)
	{
		if (States.Contains(EUnitState::Burn))
		{
			if (!BurnEffect->IsActive())
				BurnEffect->Activate();
		}
		else
		{
			BurnEffect->Deactivate();
		}
	}


	if (StunEffect)
	{
		if (States.Contains(EUnitState::Stun))
		{
			if (!StunEffect->IsActive())
				StunEffect->Activate();
		}
		else
		{
			StunEffect->Deactivate();
		}
	}


	if (PoisoningEffect)
	{
		if (States.Contains(EUnitState::Poisoning))
		{
			if (!PoisoningEffect->IsActive())
			{
				PoisoningEffect->Activate();
			}
		}
		else
		{
			PoisoningEffect->Deactivate();
		}
	}
}
