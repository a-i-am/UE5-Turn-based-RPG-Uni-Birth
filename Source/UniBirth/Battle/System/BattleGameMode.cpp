// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/System/BattleGameMode.h"
#include "Battle/BattleManager.h"
#include "Kismet/GameplayStatics.h"
#include  "Character/UBPlayer.h"
#include  "Common/Spawn/UBSpawnManager.h"
#include "UI/Combat/UBBattleHUD.h"
#include "Component/UBCharacterUI.h"
#include "Battle/SampleCharacter.h"
#include "GameFramework/GameUserSettings.h"

void ABattleGameMode::BeginPlay() {
    Super::BeginPlay();

    if (GEngine)
    {
        if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
        {
            Settings->SetOverallScalabilityLevel(1);
            Settings->ApplySettings(false);
        }
    }



    AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ABattleManager::StaticClass());
    if (actor == nullptr)
        return;
    battleManager = Cast<ABattleManager>(actor);
    AUBSpawnManager* spawnPoint = Cast<AUBSpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(),
        AUBSpawnManager::StaticClass()));
    if (!spawnPoint)
    {


        return;
    }

    spawnPoint->SpawnPlayer();
    spawnPoint->SpawnMonster();
    battleManager->InitBattle();

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    AUBBattleHUD* MyHUD = Cast<AUBBattleHUD>(PC->GetHUD());
    if (!MyHUD)
        return;
    //if (ASampleCharacter* character = Cast<ASampleCharacter>(battleManager->currentCharacter))
    //MyHUD->CharacterUI->ChangeOwner(character)
    battleManager->StartBattle();
    MyHUD->InitHUD();
   
}

ABattleManager* ABattleGameMode::GetBattleManager()
{
    return battleManager;
}

TSubclassOf<AUBPlayer> ABattleGameMode::GetPlayerClass(ECharacterType Type) const
{
    switch (Type)
    {
    case ECharacterType::CT_Uni:
        return UniPlayerClass;
    case ECharacterType::CT_Tau:
        return TauPlayerClass;
    case ECharacterType::CT_Elvasia:
        return ElvasiaPlayerClass;
    default:
        return nullptr;
    }
}
