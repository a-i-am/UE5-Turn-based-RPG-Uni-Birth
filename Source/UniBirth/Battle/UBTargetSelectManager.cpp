// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/UBTargetSelectManager.h"
#include  "BattleManager.h"
void UUBTargetSelectManager::Init(ABattleManager* InBattleManager)
{
	battleManager = InBattleManager;
	bActive = false;
	currentIndex = -1;
}

void UUBTargetSelectManager::MoveTarget(int32 Direction)
{
}
void  UUBTargetSelectManager::Activate()
{
	if (!battleManager) return;

	bActive = true;
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("in Selector"));
	//CurrentIndex = FindFirstEnemyIndex();   // 현재 characters 배열 기반
	//UpdateHighlight();
}
void  UUBTargetSelectManager::Deactivate()
{
	bActive = false;
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("Out Selector"));
}
void UUBTargetSelectManager::ConfirmTarget()
{
}

void UUBTargetSelectManager::UpdateHighlight()
{
}

int32 UUBTargetSelectManager::FindFirstEnemyIndex()
{
	return int32();
}
