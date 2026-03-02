// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Combat/UBBattleHUD.h"
#include "UI/Combat/UBCharacterPortraitList.h"
#include "Battle/BattleManager.h"
#include "UI/Combat/UBBattleMainUI.h"
#include "UI/Combat/UBBattleMonsterHP.h"
#include "Character/Enemy/UBBaseMonster.h"
#include "Battle/System/BattleGameMode.h"
#include "Component/UBPlayerWidgetComponent.h"
#include "Component/UBCharacterUI.h"

AUBBattleHUD::AUBBattleHUD()
{
}

void AUBBattleHUD::BeginPlay() {
	Super::BeginPlay();
    //게임모드에서 호출
    //InitHUD();
    
}

void AUBBattleHUD::OnTimeDelayEnded_Internal()
{
    HideScreenGuardUI();
}

void AUBBattleHUD::InitHUD()
{
    if (MainUIClass)
    {
        MainUIWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), MainUIClass);
        MainUIWidget->AddToViewport();
        if (UUBBattleMainUI* mainUI = Cast<UUBBattleMainUI>(MainUIWidget)) {
            if (ABattleGameMode* GM = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode()))
            {
                if (ABattleManager* BM = GM->GetBattleManager())
                {
                    BM->OnTimeDelayEnded.AddUObject(this, &AUBBattleHUD::OnTimeDelayEnded_Internal);
                    BM->turnOrderListWidget = mainUI->CharacterPortraitList;
                    BM->BuildOrderList();
                    mainUI->AddMonsterHPBar();
                }
            }
        }

    }

    CharacterUI = GetWorld()->SpawnActor<AUBCharacterUI>(CharacterUIClass);
}


