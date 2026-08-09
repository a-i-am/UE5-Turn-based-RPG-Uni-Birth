#include "Character/Enemy/UBBaseMonster.h"
#include "Component/ActionSystemComponent.h"
#include "Battle/System/UBAIController.h"
#include "Character/UBPlayer.h"
#include "Component/UBStatsComponent.h"
#include "Components/CapsuleComponent.h"
#include "Battle/BattleManager.h"
#include "Animation/Data/FBossPhaseEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Component/HealthComponent.h"
#include "Common/SubSystem/UBDamageSubsystem.h"
#include "Component/UBComboBuffComponent.h"
#include "Component/UBBuffComponent.h"

AUBBaseMonster::AUBBaseMonster()
{
	teamType = ETeamType::Enemy;
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AUBAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AUBBaseMonster::BeginPlay()
{
	Super::BeginPlay();

	currentState = EActionState::Idle;
	OriginalLocation = GetActorLocation();
	HealthComp->OnShieldDestroy.AddUObject(this, &AUBBaseMonster::HandleShieldBroken);
}

void AUBBaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUBBaseMonster::StartAttack()
{
	currentState = EActionState::Attack;
	asComp = this->FindComponentByClass<UActionSystemComponent>();
	if (asComp)
	{
		asComp->OnActionSelected(CurrentActionTag);
	}
}

void AUBBaseMonster::NotifyAttackIntent()
{
	if (!CurrentTarget || CurrentTarget->bIsDead())
	{
		CurrentTarget = nullptr;
		return;
	}
	if (bm) bm->OnMonsterAttackStarted.Broadcast(CurrentTarget, CurrentSkillData);
}

void AUBBaseMonster::AttackHit()
{
	result = bm->GetReactionResult();

	const FCharacterActionFXOverride* HitFX = nullptr;
	if (FXProfile)
	{
		HitFX = FXProfile->Find(CurrentActionTag);
	}

	bm->HandleMonsterAttackResult(this, ResolvedTargets.Num() > 0 ? ResolvedTargets[0] : nullptr, result);

	for (AUBCombatUnit* Target : ResolvedTargets)
	{
		int32 FinalDamage = damageSST->CalculateDamage(this, Target, CurrentSkillData);
		bm->OnHitAciton.Broadcast(Target, result);

		if (Target->currentState == EActionState::Guard)
		{
			HealthComp->SpawnGuardParticle(Target);
			HealthComp->ApplyDamage(FinalDamage, Target, nullptr, FVector::ZeroVector, FRotator::ZeroRotator, CurrentSkillData->hitCount);
			continue;
		}
		if (result == EResultType::Success)
		{
			HealthComp->ApplyDamage(FinalDamage, Target, nullptr, FVector::ZeroVector, FRotator::ZeroRotator, CurrentSkillData->hitCount);
			continue;
		}

		AUBPlayer* PlayerUnit = Cast<AUBPlayer>(Target);
		if (PlayerUnit)
		{
			PlayerUnit->BuffComp->DeleteBuffByReason(EDelCase::SkillHit);
		}
		HealthComp->ApplyDamage(FinalDamage, Target, HitFX, HitFX->ImpactScale, HitFX->ImpactRotator, CurrentSkillData->hitCount);

		if (FinalDamage > 0 && !Target->bIsDead())
		{
			if (UGameInstance* GI = GetGameInstance())
			{
				UUBSkillManager* SM = GI->GetSubsystem<UUBSkillManager>();
				if (SM)
				{
					SM->OnUnitHitDamage(this, Target, FinalDamage);
				}
			}
		}
	}
}

void AUBBaseMonster::StartAttackSequence(FGameplayTag InActionTag, AUBCombatUnit* InTargetUnit)
{
	if (!InTargetUnit || InTargetUnit->bIsDead())
	{
		return;
	}
	CurrentActionTag = InActionTag;
	CurrentTarget = InTargetUnit;
	CurrentSkillData = ResolveSkillDataFromActionTag(InActionTag);

	if (CurrentSkillData == nullptr) return;

	SkillManager->RequestSkillUse(this, CurrentSkillData->skill_key, InTargetUnit);
	ResolveTargetsFromSkill();

	if (CurrentSkillData->AttackType == 1)
	{
		StartMove();
	}
	else
	{
		StartAttack();
	}
}

void AUBBaseMonster::StartMove()
{
	currentState = EActionState::MovingToTarget;
	MoveTargetLocation = CurrentTarget->GetActorLocation();

	GetWorld()->GetTimerManager().SetTimer(
		MovementTimerHandle,
		this,
		&AUBBaseMonster::MoveToTarget,
		0.016f,
		true
	);
}

void AUBBaseMonster::AllPlayerIdle()
{
	for (AUBCombatUnit* Target : ResolvedTargets)
	{
		if (Target)
		{
			if (Target->bIsDead() == false)
				Target->currentState = EActionState::Idle;
		}
	}
}

void AUBBaseMonster::MoveToTarget()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector Curr = GetActorLocation();
	FVector Dir = (MoveTargetLocation - Curr).GetSafeNormal();

	FVector Next = Curr + (Dir * statsComp->currentStats.speed * DeltaTime);
	SetActorLocation(Next);

	if (FVector::Dist(Next, MoveTargetLocation) <= 400.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
		StartAttack();
	}
}

void AUBBaseMonster::MonsterOnActionFinished()
{
	if (this->bIsDeath == true)
	{
		return;
	}

	AUBPlayer* PlayerUnit = Cast<AUBPlayer>(CurrentTarget);
	if (!PlayerUnit)
	{
		return;
	}

	switch (CurrentTarget->currentState)
	{
	case EActionState::Guard:
		PlayerUnit->ComboBuffComponent->RequestAddComboBuff(EComboType::Guard);
		break;
	case EActionState::Dodge:
		if (result == EResultType::Success)
		{
			PlayerUnit->ComboBuffComponent->RequestAddComboBuff(EComboType::Dodge);
		}
		else if (result == EResultType::Fail)
		{
			PlayerUnit->ComboBuffComponent->ResetBuffs(EComboType::Dodge, EDelCase::ComboFailure);
		}
		break;
	case EActionState::Parry:
		if (result == EResultType::Success)
		{
			PlayerUnit->ComboBuffComponent->RequestAddComboBuff(EComboType::Parry);
		}
		else if (result == EResultType::Fail)
		{
			PlayerUnit->ComboBuffComponent->ResetBuffs(EComboType::Parry, EDelCase::ComboFailure);
		}
		break;
	}

	CurrentTarget->result = EResultType::None;

	if (MonsterType == EMonsterType::Melee)
	{
		StartReturnMove();
	}
	else
	{
		StartRotateMeshToOrigin();
	}
}

void AUBBaseMonster::StartReturnMove()
{
	currentState = EActionState::Returning;
	GetWorld()->GetTimerManager().SetTimer(
		ReturnTimerHandle,
		this,
		&AUBBaseMonster::BeforeLocation,
		0.016f,
		true
	);
}

void AUBBaseMonster::BeforeLocation()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FVector Curr = GetActorLocation();
	FVector Dir = (OriginalLocation - Curr).GetSafeNormal();

	FVector Next = Curr + (Dir * statsComp->currentStats.speed * DeltaTime);
	SetActorLocation(Next);

	if (FVector::DistSquared(Next, OriginalLocation) <= 25.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReturnTimerHandle);
		SetActorLocation(OriginalLocation);
		StartRotateMeshToOrigin();
	}
}

void AUBBaseMonster::StartRotateToTarget(AUBCombatUnit* InTargetUnit)
{
	OriginalLocation = GetActorLocation();
	OriginRotator = GetMesh()->GetRelativeRotation();

	GetWorld()->GetTimerManager().ClearTimer(RotateTimerHandle);

	FVector From = GetActorLocation();
	FVector To = InTargetUnit->GetActorLocation();

	FVector Dir = To - From;
	Dir.Z = 0;

	TargetRotator = Dir.Rotation();
	TargetRotator.Yaw += 90.f;

	GetWorld()->GetTimerManager().SetTimer(
		RotateTimerHandle,
		this,
		&AUBBaseMonster::RotateTickToTarget,
		0.016f,
		true
	);
}

void AUBBaseMonster::RotateTickToTarget()
{
	InterpRotateTo(TargetRotator, RotateSpeed, RotateTimerHandle);
}

void AUBBaseMonster::RotateTickToOrigin()
{
	InterpRotateTo(OriginRotator, RotateSpeed, RotateToOriginTimerHandle);
	if (result == EResultType::Success)
	{
		for (AUBCombatUnit* PlayerUnit : ResolvedTargets)
		{
			if (PlayerUnit->currentState == EActionState::Parry)
			{
				PlayerUnit->Counter(this);
			}
		}
	}
	AllPlayerIdle();
}

void AUBBaseMonster::StartRotateMeshToOrigin()
{
	if (bIsDeath == true) return;
	currentState = EActionState::Idle;
	OnActionFinishedDelegate.Broadcast();
	OnActionFinishedDelegate.Clear();
	GetWorld()->GetTimerManager().ClearTimer(RotateToOriginTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		RotateToOriginTimerHandle,
		this,
		&AUBBaseMonster::RotateTickToOrigin,
		0.016f,
		true
	);
}

void AUBBaseMonster::HandleShieldBroken()
{
	bIsPlaying = true;
}

void AUBBaseMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AUBBaseMonster::ResolveTargetsFromSkill()
{
	ResolvedTargets.Empty();

	if (!CurrentSkillData) return;

	if (CurrentSkillData->target_scope == 1)
	{
		if (CurrentTarget)
		{
			ResolvedTargets.Add(CurrentTarget);
			StartRotateToTarget(ResolvedTargets[0]);
		}
	}
	else if (CurrentSkillData->target_scope == 3)
	{
		ResolvedTargets = bm->GetAllPlayer();
	}
}

FCharacterSkill* AUBBaseMonster::ResolveSkillDataFromActionTag(const FGameplayTag& InActionTag)
{
	if (InActionTag == UBGameplayTags::Action_Monster_AttackA)
		return FindSkillKey(TEXT("SKILL_ACTIVE1"));

	if (InActionTag == UBGameplayTags::Action_Monster_AttackB)
		return FindSkillKey(TEXT("SKILL_ACTIVE2"));

	if (InActionTag == UBGameplayTags::Action_Monster_AttackC)
		return FindSkillKey(TEXT("SKILL_ACTIVE3"));

	if (InActionTag == UBGameplayTags::Action_Monster_AttackD)
		return FindSkillKey(TEXT("SKILL_ACTIVE4"));

	return nullptr;
}

void AUBBaseMonster::InterpRotateTo(const FRotator& InTargetRot, float InSpeed, FTimerHandle& InTimerHandle)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FRotator Curr = GetMesh()->GetRelativeRotation();
	FRotator Next = FMath::RInterpConstantTo(Curr, InTargetRot, DeltaTime, InSpeed);

	GetMesh()->SetRelativeRotation(Next);

	if (Next.Equals(InTargetRot, 0.5f))
	{
		GetMesh()->SetRelativeRotation(InTargetRot);
		GetWorld()->GetTimerManager().ClearTimer(InTimerHandle);
	}
}

void AUBBaseMonster::ShieldRefill()
{
	if (bIsRefill == true) return;

	if (characterType == ECharacterType::Boss)
	{
		AUBAIController* AIController = Cast<AUBAIController>(GetController());
		statsComp->currentStats.shield = (statsComp->stats.shield * 0.3f);

		statsComp->currentStats.Mp += 20;
		bIsRefill = true;
	}
}

void AUBBaseMonster::DeathCharacter()
{
	bIsDeath = true;
	currentState = EActionState::Die;
	asComp->OnActionSelected(UBGameplayTags::Action_Death);
	bm->DeathCharacterReMove(this);
}

