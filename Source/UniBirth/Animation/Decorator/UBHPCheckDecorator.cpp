// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Decorator/UBHPCheckDecorator.h"
#include "Character/Enemy/UBBaseMonster.h"
#include "Battle/System/UBAIController.h"
#include "Component/UBStatsComponent.h"
#include "Common/SubSystem/UBSkillManager.h"

UUBHPCheckDecorator::UUBHPCheckDecorator()
{
	NodeName = "Check Hp";
}

bool UUBHPCheckDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return false;

	AUBBaseMonster* Monster = Cast<AUBBaseMonster>(AI->GetPawn());
	if (!Monster || !Monster->statsComp) return false;

	Monster->currentPhase = currentBossPhase;
	float CurrentHP = Monster->statsComp->currentStats.Hp;
	float MaxHp = Monster->statsComp->currentStats.MaxHP;

	// 몬스터가 특정 페이즈에 진입했을 때 모드 체크(왕의 폭주)
	bool bConditionMet = CurrentHP <= MaxHp * RequiredHp;

	if (bConditionMet)
	{
		if (UGameInstance* GI = Monster->GetGameInstance())
		{
			UUBSkillManager* SkillManger = GI->GetSubsystem<UUBSkillManager>();
			
			if (SkillManger)
			{
				SkillManger->CheckPassive(Monster);
			}
		}

		return bConditionMet;
	}

	return CurrentHP <= MaxHp * RequiredHp;
}

UUBMpCheckDecorator::UUBMpCheckDecorator()
{
	NodeName = "Mp Check";
}

bool UUBMpCheckDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return false;

	AUBBaseMonster* Monster = Cast<AUBBaseMonster>(AI->GetPawn());
	if (!Monster || !Monster->statsComp) return false;

	float CurrentMp = Monster->statsComp->currentStats.Mp;
	return CurrentMp >= RequiredMp;

}

UUBCheckStatsDecorator::UUBCheckStatsDecorator()
{
}

bool UUBCheckStatsDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return false;

	AUBBaseMonster* Monster = Cast<AUBBaseMonster>(AI->GetPawn());
	if (!Monster || !Monster->statsComp) return false;

	//@TODO
	return false;
}

UUBCheckLastSkillDecorator::UUBCheckLastSkillDecorator()
{
	NodeName = "LastSkillCheck";
}

bool UUBCheckLastSkillDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return false;

	AUBBaseMonster* Monster = Cast<AUBBaseMonster>(AI->GetPawn());
	if (Monster == nullptr) return false;
	if (Monster->lastSkillList.Num() == 0) return true;

	int32 NumSkill = Monster->lastSkillList.Num();
	int32 CheckCount = FMath::Min(range, NumSkill);

	for (int i = 0; i < CheckCount; ++i)
	{
		int32 index = NumSkill - 1 - i;
		if (Monster->lastSkillList[index] == lastSkillTag)
		{
			return false;
		}
	}

	return true;

}

