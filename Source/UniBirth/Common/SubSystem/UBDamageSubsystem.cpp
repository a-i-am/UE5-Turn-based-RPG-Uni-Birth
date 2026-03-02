// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SubSystem/UBDamageSubsystem.h"
#include "Character/Interface/UBCombatUnit.h"
#include "Battle/SampleCharacter.h"
#include "GameplayTagContainer.h"
#include "Component/UBStatsComponent.h"
#include "Character/Data/UBCharacterSkillData.h"
UUBDamageSubsystem::UUBDamageSubsystem()
{
}

// 상태이상확률
bool UUBDamageSubsystem::CheckState(AUBCombatUnit* Unit)
{
	float ResistPercent = Unit->statsComp->currentStats.ResistRate; 
	float RandPercent = FMath::FRandRange(0.f, 1.f);

	return RandPercent < ResistPercent;
}

bool UUBDamageSubsystem::CheckCritical(AUBCombatUnit* Unit)
{
	float CrtPercent = Unit->statsComp->currentStats.critRate;
	float RandPercent = FMath::FRandRange(0.f, 1.f);
	
	return CrtPercent > RandPercent;
}

int32 UUBDamageSubsystem::CalculateDamage(AUBCombatUnit* Attacker, AUBCombatUnit* Target, const FCharacterSkill* Skill)
{

	float BaseDamage;

	if (CheckCritical(Attacker))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("!!!crt atk"));
		BaseDamage = (Attacker->statsComp->currentStats.Attack * Skill->DamageRate) * 1.5f;
	}
	else
	{
		BaseDamage = Attacker->statsComp->currentStats.Attack * Skill->DamageRate;
	}
	switch (Target->currentState)
	{
	case EActionState::Guard:
		BaseDamage *= 0.5;
		break;
	case EActionState::Dodge:
		if (Target->result == EResultType::Success)
		{
			BaseDamage = 0;
		}
		else
		{
			BaseDamage *= 1.2f;
		}
		break;
	case EActionState::Parry:
		if (Target->result == EResultType::Success)
		{
			BaseDamage = 0.f;
		}
		else if (Target->result == EResultType::Fail)
		{
			BaseDamage *= 1.2f;
		}
		break;

	}
	BaseDamage -= Target->statsComp->currentStats.Defence;
	return FMath::Max(0, FMath::RoundToInt(BaseDamage));
}
int32 UUBDamageSubsystem::CalculateNormalAttackDamage(AUBCombatUnit* Attacker, AUBCombatUnit* Target)
{
	float BaseDamage;
	if (CheckCritical(Attacker))
	{
		BaseDamage = Attacker->statsComp->currentStats.Attack * 1.5;
	}
	else
	{
		BaseDamage = Attacker->statsComp->currentStats.Attack;
	}
	return FMath::Max(0, FMath::RoundToInt(BaseDamage));

}



