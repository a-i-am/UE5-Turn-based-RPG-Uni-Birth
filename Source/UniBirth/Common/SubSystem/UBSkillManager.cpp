// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SubSystem/UBSkillManager.h"
#include "Character/Interface/UBCombatUnit.h"
#include "Common/System/UBGameInstance.h"
#include "Component/UBStatsComponent.h"
#include "Component/UBBuffComponent.h"

void UUBSkillManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UUBSkillManager::TestRequestSkillUse(AUBCombatUnit* user, const FString& Key, AUBCombatUnit* Target)
{
	RequestSkillUse(user, Key, Target);
}

FCharacterSkill* UUBSkillManager::RequestSkillUse(AUBCombatUnit* user, const FString& Key, AUBCombatUnit* Target)
{
	auto skill = user->FindSkillKey(Key);

	if (skill == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("null skills"));
		return nullptr;
	}
	if (user->statsComp->currentStats.Mp < skill->mp_cost)
	{
		return nullptr;
	}
	user->statsComp->currentStats.Mp -= (skill->mp_cost);

	UseSkillEffect(user, skill, Target);
	SetAtvSkill(true);
	return skill;
}

void UUBSkillManager::ApplyStatEffect(AUBCombatUnit* Unit, const FSkillBuffData& Buff)
{
	FBuffSlot newSlot;
	newSlot.ID = Buff.ID;
	newSlot.Icon = Buff.Icon;
	newSlot.priority = EBuffPriority::Skill;
	newSlot.CurrTurnCount = 0;
	newSlot.TurnDurationCount = Buff.TurnDurCount;
	newSlot.DurType = Buff.DurType;
	newSlot.BuffStats = Buff.BuffStats;
	newSlot.Values = Buff.Values;
	newSlot.StatOperator = Buff.StatOperator;
	newSlot.State = Buff.State;
	newSlot.StackRule = Buff.StackRule;
	newSlot.MaxStackCount = Buff.StackCount;
	newSlot.CurrentStack = 1;
	newSlot.BuffType = Buff.BuffType;
	newSlot.MakeDelCase(Buff.DelCase);
	Unit->BuffComp->AddBuffToStat(newSlot, true);
}

void UUBSkillManager::UseSkillEffect(AUBCombatUnit* Unit, const FCharacterSkill* SkillMeta, AUBCombatUnit* Target)
{
	if (!SkillMeta || SkillMeta->skill_buffs.IsEmpty())
	{
		return;
	}

	TArray<FString> OutArray;

	AUBCombatUnit* ApplyTarget;
	if (SkillMeta->teamType == ETeamType::Ally_Team)
	{
		ApplyTarget = Unit;
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("APPLY"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("ENEMY"));
		ApplyTarget = Target;
	}
	SkillMeta->skill_buffs.ParseIntoArray(OutArray, TEXT(","), true);

	for (FString buffStrID : OutArray)
	{
		int32 buffID = FCString::Atoi(*buffStrID);
		if (UUBGameInstance* gameInstance = GetWorld()->GetGameInstance<UUBGameInstance>())
		{
			FSkillBuffData* buffData = gameInstance->GetSkillBuffData(buffID);
			if (buffData)
			{
				ApplyStatEffect(ApplyTarget, *buffData);
			}
		}
	}

}

void UUBSkillManager::CheckPassive(AUBCombatUnit* unit)
{
	for (const FCharacterSkill& Passive : unit->OwnedPassvieSkills)
	{
		if (!CheckPassiveCondition(unit, Passive.PassiveCondition))
		{
			continue;
		}

		UseSkillEffect(unit, &Passive, unit);
	}
}

bool UUBSkillManager::CheckPassiveCondition(AUBCombatUnit* Unit, EPassiveCondition condtion)
{
	switch (condtion)
	{
	case EPassiveCondition::HasShield:
		return Unit->statsComp->currentStats.shield <= 0;
	case EPassiveCondition::UsedSkillThisTurn:
		return AtvSkill == true;
	case EPassiveCondition::HpBelow50:
		return Unit->statsComp->currentStats.Hp<= (Unit->statsComp->currentStats.MaxHP * 0.5);
	case EPassiveCondition::HpBelow40:
		return Unit->statsComp->currentStats.Hp<= (Unit->statsComp->currentStats.MaxHP * 0.4);
	case EPassiveCondition::StateEffect:
		return 0;
	}
	return false;
}
void UUBSkillManager::SetAtvSkill(bool Value)
{
	AtvSkill = Value;
}
void UUBSkillManager::OnTurnStart(AUBCombatUnit* Unit)
{
	SetAtvSkill(false);
	CheckPassive(Unit);    // 패시브 전부 재적용
}

void UUBSkillManager::OnUnitHitDamage(AUBCombatUnit* Attacker, AUBCombatUnit* Victim, int32 Damage)
{
	if (Damage <= 0 || !Attacker || !Victim)
	{
		return;
	}

	for (const FCharacterSkill& Passive : Attacker->OwnedPassvieSkills)
	{
		if (Passive.PassiveCondition == EPassiveCondition::StateEffect)
		{
			UseSkillEffect(Attacker, &Passive, Victim);
		}
	}
}