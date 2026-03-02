// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Character/Data/UBCharacterSkillData.h"
#include "UBDamageSubsystem.generated.h"


class AUBCombatUnit;
/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBDamageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UUBDamageSubsystem();

	bool CheckState(AUBCombatUnit* Unit);
	
	bool CheckCritical(AUBCombatUnit* Unit);
	
	//UFUNCTION()
	int32 CalculateDamage(AUBCombatUnit* Attacker,AUBCombatUnit* Defender, const FCharacterSkill* skill);

	int32 CalculateNormalAttackDamage(AUBCombatUnit* Attacker, AUBCombatUnit* Target);


};
