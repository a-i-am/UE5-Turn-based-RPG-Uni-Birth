// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/SampleCharacter.h"
#include "Battle/System/BattleGameMode.h"
#include "Battle/BattleManager.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Component/ActionSystemComponent.h"
#include "Component/UBStatsComponent.h"
#include "Component/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/Data/UBCharacterFXProfile.h"
#include "Character/Data/UBCharacterSkillData.h"
#include "Common/System/UBGameInstance.h"
#include "Common/SubSystem/UBSkillManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Component/UBComboBuffComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Common/SubSystem/UBDamageSubsystem.h"
// Sets default values
ASampleCharacter::ASampleCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASampleCharacter::BeginPlay()
{
	Super::BeginPlay();
	auto pc = Cast<APlayerController>(Controller);
	if (pc) {
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem) {
			subsystem->AddMappingContext(inputMapping.LoadSynchronous(), 0);
		}
	}

	InitialMeshRelativeLocation = GetMesh()->GetRelativeLocation();
	InitialMeshRelativeRotation = GetMesh()->GetRelativeRotation();

	
	//델리게이트 바인딩
	bm->OnHitAciton.AddUObject(this, &ASampleCharacter::StartParryingAction);
	bm->OnMonsterAttackResult.AddUObject(this, &ASampleCharacter::HandleMonsterAttackResolved);
	if (asComp == nullptr) return;
	OriginalLocation = GetActorLocation();
	currentState = EActionState::idle;
	
}


void ASampleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

bool ASampleCharacter::operator<(const ASampleCharacter& Other) const
{
	return statsComp->currentStats.AttackSpeed >
		Other.statsComp->currentStats.AttackSpeed;
}

void ASampleCharacter::Attack(AUBCombatUnit* unit)
{
	if (!bIsMyTurn)
		return;
	if (!asComp)
	{
		return;
	}
	if (bm == nullptr)
	{
		return;
	}

	if (unit == nullptr)
	{
		return;
	}
	currentState = EActionState::Attack;
	CurrentTarget = unit;

	CurrentActionTag = UBGameplayTags::Action_Attack_Normal;
	asComp->OnActionSelected(CurrentActionTag);

}

//스킬 시작전 체크하는 로직 ex) target , Mp 등등 
void ASampleCharacter::ActiveSkill(AUBCombatUnit* unit)
{
	CurrentTarget = unit;
	currentState = EActionState::ActiveSkill;

	CurrentActionTag = UBGameplayTags::Action_Attack_Skill;

	gameInstance->SkillManager->RequestSkillUse(this, TEXT("SKILL_ACTIVE"), CurrentTarget);
	
	asComp->OnActionSelected(CurrentActionTag);
}

void ASampleCharacter::UltiMateKSill(AUBCombatUnit* unit)
{
	CurrentTarget = unit;
	currentState = EActionState::UltiMateSkill;
	CurrentActionTag = UBGameplayTags::Action_Attack_Ultimate;
	asComp->OnActionSelected(CurrentActionTag);
}

void ASampleCharacter::Defence()
{ 
	CurrentActionTag = UBGameplayTags::Action_Defence;
	currentState = EActionState::Guard;
	bm->SetGlobalTimeReset();
	ResultGuard(EResultType::Success);
}

void ASampleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) 
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
//내가 때린 대상
void ASampleCharacter::AttackHit()
{	
	const FCharacterActionFXOverride* FX = nullptr;
	if (this->FXProfile)
	{
		FX = this->FXProfile->Find(CurrentActionTag);
	}
	if (FX == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("NULL"));
		return;
	}
		
	//일반공격 데미지처리
	if (CurrentActionTag == UBGameplayTags::Action_Attack_Normal || CurrentActionTag == UBGameplayTags::Action_Counter)
	{
		
		//평타데미지에 대한 처리 
		int32 FinalDamage = damageSST->CalculateNormalAttackDamage(this, CurrentTarget);
		HealthComp->ApplyDamage(FinalDamage, CurrentTarget, FX, FX->ImpactScale, FX->ImpactRotator, NormalAttackHitCount);
	}
	// 궁극기 공격 데미지 처리 
	else
	{
		FCharacterSkill* Skills = gameInstance->SkillManager->RequestSkillUse(this, TEXT("SKILL_ULTIMATE"), CurrentTarget);
		int32 FinalDamage = damageSST->CalculateDamage(this, CurrentTarget, Skills);
		HealthComp->ApplyDamage(FinalDamage, CurrentTarget, FX, FX->ImpactScale, FX->ImpactRotator, 1);
	}
	if(statsComp->currentStats.CurrState.Contains( EUnitState::Stealth))
	EndStealth();
}

void ASampleCharacter::SetActionState(EActionState State)
{
	currentState = State;
}

void ASampleCharacter::SkipTurn()
{
	if (!bIsMyTurn)
		return;

	if (ABattleGameMode* GM = GetWorld()->GetAuthGameMode<ABattleGameMode>())
	{
		if (ABattleManager* BM = GM->GetBattleManager())
		{
			BM->NextTurn();
		}
	}
}
void ASampleCharacter::RotateToTarget(AUBCombatUnit* Unit)
{
	if (!Unit) return;
	CurrentTarget = Unit;
	SavedRotBeforeAttack = GetMesh()->GetRelativeRotation();
	bRestoreRotationAfterAttack = true;
	FVector From = GetActorLocation();
	FVector To = Unit->GetActorLocation();

	FVector Dir = To - From;
	Dir.Z = 0;

	FRotator WorldRot = Dir.Rotation();
	WorldRot.Yaw -= 90.f;
	GetMesh()->SetRelativeRotation(WorldRot);
}

void ASampleCharacter::OnActionFinished()
{
	if (!asComp || !bm)return;
	//공격 전으로 돌리기
	if (bRestoreRotationAfterAttack)
	{
		GetMesh()->SetRelativeRotation(SavedRotBeforeAttack);
		bRestoreRotationAfterAttack = false;
	}
	bm->HandleGameEnd();
	// 공격일때만 밑에 로직 실행
	if (
		CurrentActionTag != UBGameplayTags::Action_Attack_Normal &&
		CurrentActionTag != UBGameplayTags::Action_Attack_Skill &&
		CurrentActionTag != UBGameplayTags::Action_Attack_Ultimate
		)
		return;
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->SetViewTargetWithBlend(this, 0.2f);
	}
	if (statsComp->currentStats.CurrState.Contains(EUnitState::Stealth))
	{
		if(isStealth == true)
		EndStealth();
	}
	SkillManager->CheckPassive(this);
	HealMp(1);
	currentState = EActionState::idle;
	OnActionFinished_.Broadcast();
	bm->NextTurn();
}

void ASampleCharacter::HandleMonsterAttackResolved(AUBCombatUnit* Attacker, AUBCombatUnit* Target, EResultType Result)
{
	if (bIsDeath == true)
	{
		return;
	}
	bool bIsIdle = (currentState == EActionState::idle);
	if (bIsIdle)
	{
		CurrentActionTag = UBGameplayTags::Action_Hit;
		asComp->OnActionSelected(CurrentActionTag);
		return;
	}

	switch (currentState)
	{
	case EActionState::Dodge:
		ResultDodge(Result);
		break;
	case EActionState::Parry:
		ResultParrying(Result);
		break;
	}
}
void ASampleCharacter::ResultGuard(EResultType Result)
{
	CurrentActionTag = UBGameplayTags::Action_Defence;
	asComp->OnActionSelected(CurrentActionTag);
}
void ASampleCharacter::ResultParrying(EResultType Result)
{
	result = Result;
	if (result == EResultType::Fail)
	{
		CurrentActionTag = UBGameplayTags::Action_Hit;
		asComp->OnActionSelected(CurrentActionTag);
	}
	else  if (result == EResultType::Success)
	{
		//성공
		CurrentActionTag = UBGameplayTags::Action_Parry;
		asComp->OnActionSelected(CurrentActionTag);
	}
}
void ASampleCharacter::ResultDodge(EResultType Result)
{
	result = Result;
	if (result == EResultType::Fail)
	{
		CurrentActionTag = UBGameplayTags::Action_Hit;
		asComp->OnActionSelected(CurrentActionTag);
	}
	else if (result == EResultType::Success)
	{
		CurrentActionTag = UBGameplayTags::Action_Dodge;
		asComp->OnActionSelected(CurrentActionTag);
	}
}

//반격
void ASampleCharacter::Counter(AUBCombatUnit* target)
{
	CurrentActionTag = UBGameplayTags::Action_Counter;
	CurrentTarget = target;

	RotateToTarget(target);
	asComp->OnActionSelected(CurrentActionTag);
}
void ASampleCharacter::StartParryingAction(AUBCombatUnit* unit, EResultType Result)
{
	if (Result == EResultType::Fail) return;
	if (this != unit) return;
	switch (currentState)
	{
	case EActionState::Parry:
		asComp->OnActionSelected(UBGameplayTags::Action_Parry);
		break;
	case EActionState::Dodge:
		asComp->OnActionSelected(UBGameplayTags::Action_Dodge);
		break;
	}
}
void ASampleCharacter::MoveTowardTargetFromSocket(
	const FVector& CurrentSocketWS,
	AUBCombatUnit* Target,
	float DeltaTime)
{
	float MoveSpeed = 2000.f; // 원하는 값
	InitialMeshRelativeLocation.Z = -90;
	if (!Target) return;

		FVector ToTarget = Target->GetActorLocation() - GetActorLocation();
		ToTarget.Z = 0.f;
		const float AttackArriveSq = 50.f * 50.f;
	
		// 도착  복귀로 전환
		if (ToTarget.SizeSquared() <= AttackArriveSq)
		{
			return;
		}

		if (UCharacterMovementComponent* Move = GetCharacterMovement())
		{
			FVector Dir = ToTarget.GetSafeNormal();
			Move->MoveUpdatedComponent(Dir * MoveSpeed * DeltaTime, GetActorRotation(), false);
		}

}

void ASampleCharacter::DeathCharacter()
{
	bIsDeath = true;
	currentState = EActionState::Die;
	asComp->OnActionSelected(UBGameplayTags::Action_Death);
	bm->DeathCharacterReMove(this);

}

void ASampleCharacter::CharacterResetLocation(const FVector& CurrentSocketWS,float DeltaTime)
{
	float MoveSpeed = 2000.f; // 원하는 값
	FVector ToOrigin = OriginalLocation - GetActorLocation();
	InitialMeshRelativeLocation.Z = -90;
	ToOrigin.Z = 0.f;
	const float ArriveOriginSq = 30.f * 30.f;

	if (ToOrigin.SizeSquared() < ArriveOriginSq)
	{
		GetMesh()->SetRelativeLocation(InitialMeshRelativeLocation);
		return;
	}
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		FVector Dir = ToOrigin.GetSafeNormal();
		Move->MoveUpdatedComponent(Dir * MoveSpeed * DeltaTime, GetActorRotation(), false);
	}

			
}




