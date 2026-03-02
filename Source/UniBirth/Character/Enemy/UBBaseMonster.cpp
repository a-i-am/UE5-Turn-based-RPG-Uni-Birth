// Fill out your copyright notice in the Description page of Project Settings.


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
// Sets default values

AUBBaseMonster::AUBBaseMonster()
{
	
	teamType = ETeamType::Enemy_Team;
	PrimaryActorTick.bCanEverTick = true;
	
	AIControllerClass = AUBAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	 
}
void AUBBaseMonster::BeginPlay()
{
	Super::BeginPlay();

	currentState = EActionState::idle;
	OriginalLocation = GetActorLocation();
	HealthComp->OnShieldDestroy.AddUObject(this, &AUBBaseMonster::HandleShieldBroken);
}

// Called every frame
void AUBBaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//어택 시작
void AUBBaseMonster::startAttack()
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
	//성공실패 반환
	result = bm->GetReactionResult();

	const FCharacterActionFXOverride* hitFX = nullptr;
	if (FXProfile)
	{
		hitFX = FXProfile->Find(CurrentActionTag);
	}

	bm->HandleMonsterAttackResult(this,ResolvedTargets.Num() > 0 ? ResolvedTargets[0] : nullptr,result);

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
		
		// 캐릭터 피격 시
		AUBPlayer* player = Cast<AUBPlayer>(Target);
		if (player)
		{
			player->BuffComp->DeleteBuffByReason(EDelCase::SkillHit);
		}
		HealthComp->ApplyDamage(FinalDamage, Target, hitFX, hitFX->ImpactScale, hitFX->ImpactRotator, CurrentSkillData->hitCount);

		if (FinalDamage > 0 && !Target->bIsDead())
		{
			if (UGameInstance* GI = GetGameInstance())
			{
				UUBSkillManager* SM = GI->GetSubsystem<UUBSkillManager>();
				if (SM)
				{
					// "공격자(this)가 대상(Target)에게 데미지를 입혔음"을 보고합니다.
					// 이를 통해 '마비 독' 같은 패시브가 자동으로 타겟에게 적용됩니다.
					SM->OnUnitHitDamage(this, Target, FinalDamage);
				}
			}
		}
	}
}

void AUBBaseMonster::StartAttackSequence(FGameplayTag ActionTag, AUBCombatUnit* target)
{
	if (!target || target->bIsDead())
	{
		return;
	}
	CurrentActionTag = ActionTag;
	CurrentTarget = target;
	CurrentSkillData = ResolveSkillDataFromActionTag(ActionTag);

	if (CurrentSkillData == nullptr)return;

	SkillManager->RequestSkillUse(this, CurrentSkillData->skill_key, target);
	ResolveTargetsFromSkill();

	//1은 근거리
	if (CurrentSkillData->AttackType == 1)
	{
		StartMove();
	}
	else
	{
		startAttack();
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
			if(Target->bIsDead() == false)
			Target->currentState = EActionState::idle;
		}
	}
}

void AUBBaseMonster::MoveToTarget()
{
	float  DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector curr = GetActorLocation();              
	FVector Dir = (MoveTargetLocation - curr).GetSafeNormal();  
	
	FVector Next = curr + (Dir * statsComp->currentStats.speed * DeltaTime);
	SetActorLocation(Next);

	if (FVector::Dist(Next, MoveTargetLocation) <= 400.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
		startAttack();
	}

}
void AUBBaseMonster::MonsterOnActionFinished()
{
	if (this->bIsDeath == true)
	{
		return;
	}

	AUBPlayer* player = Cast<AUBPlayer>(CurrentTarget);
	if (!player)
	{
		return;
	}

	switch (CurrentTarget->currentState)
	{
		case EActionState::Guard:
				player->comboBuffComp->RequestAddComboBuff(EComboType::GUARD);
			break;
		case EActionState::Dodge:
			if (result == EResultType::Success)
			{
				player->comboBuffComp->RequestAddComboBuff(EComboType::DODGE);
			}
			else if (result == EResultType::Fail)
			{
				player->comboBuffComp->ResetBuffs(EComboType::DODGE, EDelCase::ComboFailure);
			}
			break;
		case EActionState::Parry:
			if (result == EResultType::Success)
			{
				player->comboBuffComp->RequestAddComboBuff(EComboType::PARRY);
			}
			else if (result == EResultType::Fail)
			{
				player->comboBuffComp->ResetBuffs(EComboType::PARRY, EDelCase::ComboFailure);
			}
			break;
	}
	
	CurrentTarget->result = EResultType::None;

	if (monsterType == EMonsterType::Melee)
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
	float deltaTime = GetWorld()->GetDeltaSeconds();
	
	FVector Curr = GetActorLocation();
	FVector dir = (OriginalLocation - Curr).GetSafeNormal();

	FVector Next = Curr + (dir * statsComp->currentStats.speed * deltaTime);
	SetActorLocation(Next);

	if (FVector::DistSquared(Next, OriginalLocation) <= 25.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReturnTimerHandle);
		SetActorLocation(OriginalLocation);
		StartRotateMeshToOrigin();
	}
}
// 타겟 바라보기 시작
void AUBBaseMonster::StartRotateToTarget(AUBCombatUnit* Target)
{
	OriginalLocation = GetActorLocation();
	OriginRotator = GetMesh()->GetRelativeRotation();
	
	GetWorld()->GetTimerManager().ClearTimer(RotateTimerHandle);

	FVector From = GetActorLocation();
	FVector To = Target->GetActorLocation();

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
	InterpRotateTo(TargetRotator, rotateSpeed, RotateTimerHandle);
}
void AUBBaseMonster::RotateTickToOrigin()
{
	InterpRotateTo(OriginRotator, rotateSpeed, RotatoeToOrginTimerHandle);
	if (result == EResultType::Success)
	{
		for (AUBCombatUnit* Player : ResolvedTargets)
		{
			if (Player->currentState == EActionState::Parry)
			{
				Player->Counter(this);
			}
		}
	}
	AllPlayerIdle();
	 
}

// 회전복귀
void AUBBaseMonster::StartRotateMeshToOrigin()
{
	if (bIsDeath == true) return;
	currentState = EActionState::idle;
	OnActionFinishedDelegate.Broadcast();
	OnActionFinishedDelegate.Clear();
	GetWorld()->GetTimerManager().ClearTimer(RotatoeToOrginTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		RotatoeToOrginTimerHandle,
		this,
		&AUBBaseMonster::RotateTickToOrigin,
		0.016f,
		true
	);

}

void AUBBaseMonster::HandleShieldBroken()
{
	bIsPlay = true;
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
		// 단체 (전체)
		ResolvedTargets = bm->GetAllPlayer();
	}
}
//스킬 키 찾기
FCharacterSkill* AUBBaseMonster::ResolveSkillDataFromActionTag(
	const FGameplayTag& ActionTag)
{
	if (ActionTag == UBGameplayTags::Action_Monster_AttackA)
		return FindSkillKey(TEXT("SKILL_ACTIVE1"));

	if (ActionTag == UBGameplayTags::Action_Monster_AttackB)
		return FindSkillKey(TEXT("SKILL_ACTIVE2"));

	if (ActionTag == UBGameplayTags::Action_Monster_AttackC)
		return FindSkillKey(TEXT("SKILL_ACTIVE3"));

	if (ActionTag == UBGameplayTags::Action_Monster_AttackD)
		return FindSkillKey(TEXT("SKILL_ACTIVE4"));

	return nullptr;
}

void AUBBaseMonster::InterpRotateTo(
const FRotator& TargetRot,
float Speed,
FTimerHandle& TimerHandle
)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FRotator Curr = GetMesh()->GetRelativeRotation();
	FRotator Next = FMath::RInterpConstantTo(Curr, TargetRot, DeltaTime, Speed);

	GetMesh()->SetRelativeRotation(Next);

	if (Next.Equals(TargetRot, 0.5f))
	{
		GetMesh()->SetRelativeRotation(TargetRot);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}
void AUBBaseMonster::SheildRefill()
{
	if (bisRefill == true) return;

	if (characterType == ECharacterType::CT_Boss)
	{
		AUBAIController* AI = Cast<AUBAIController>(GetController());
		statsComp->currentStats.shield = (statsComp->stats.shield * 0.3f);

		statsComp->currentStats.Mp += 20;
		bisRefill = true;
	}
}


void AUBBaseMonster::DeathCharacter()
{
	bIsDeath = true;
	currentState = EActionState::Die;
	asComp->OnActionSelected(UBGameplayTags::Action_Death);
	bm->DeathCharacterReMove(this);

}