// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Character/Data/UBCharacterSkillData.h"
#include "UBSkillManager.generated.h"

UENUM(BlueprintType)
enum class EPassiveTrigger : uint8
{
	TurnStart,      // 자기 턴 시작
	AfterAction     // 스킬/공격 사용 후
};

UCLASS(Blueprintable)
class UNIBIRTH_API UUBSkillManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(EditDefaultsOnly, Category = "Skill Data")
	UDataTable* SkillTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPassiveTrigger passiveTriger;
	UFUNCTION(BlueprintCallable)
	void TestRequestSkillUse(class AUBCombatUnit* user, const FString& Key, AUBCombatUnit* Target);

	FCharacterSkill* RequestSkillUse(class AUBCombatUnit* user, const FString& Key, AUBCombatUnit* Target);
	

	bool AtvSkill = true;

	void ApplyStatEffect(AUBCombatUnit* Unit, const FSkillBuffData& Effect);

	void UseSkillEffect(AUBCombatUnit* user, const FCharacterSkill* SkillMeta, AUBCombatUnit* Target);

	void CheckPassive(AUBCombatUnit* unit);
	bool CheckPassiveCondition(AUBCombatUnit* Unit, EPassiveCondition condtion);

	void SetAtvSkill(bool Value);

	void OnTurnStart(AUBCombatUnit* Unit);

	//bool HasBuff(AUBCombatUnit* Unit, EEffectType Type);

	UPROPERTY()
	TObjectPtr<class UUBDamageSubsystem> damageSST;

	void UseMp(AUBCombatUnit* Unit, FCharacterSkill* skills);

	void OnUnitHitDamage(AUBCombatUnit* Attacker, AUBCombatUnit* Victim, int32 Damage);
};
