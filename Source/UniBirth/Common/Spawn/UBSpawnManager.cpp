// Fill out your copyright notice in the Description page of Project Settings.


#include "UBSpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Character/UBPlayer.h"
#include "UBSpawnPoint.h"
#include "UBMonsterSpawnPoint.h"
#include "Common/Data/UBSaveGame.h"
#include "Battle/System/BattleGameMode.h"
#include "Character/Enemy/UBBaseMonster.h"
// Sets default values
AUBSpawnManager::AUBSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUBSpawnManager::BeginPlay()
{
	Super::BeginPlay();
     //PlayerClass.Empty();

    if (ABattleGameMode* gamemode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode())) {
        if (UUBSaveGame* LoadedGame = Cast<UUBSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainData"), 0)))
        {
            if (LoadedGame->SelectedCharacters.Contains(ECharacterType::CT_Tau)) {
                PlayerClass.Add(gamemode->GetPlayerClass(ECharacterType::CT_Tau));
            }
            if (LoadedGame->SelectedCharacters.Contains(ECharacterType::CT_Uni)) {
                PlayerClass.Add(gamemode->GetPlayerClass(ECharacterType::CT_Uni));
            }
            if (LoadedGame->SelectedCharacters.Contains(ECharacterType::CT_Elvasia)) {
                PlayerClass.Add(gamemode->GetPlayerClass(ECharacterType::CT_Elvasia));
            }
        }
    }
    
}

// Called every frame
void AUBSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AUBSpawnManager::SpawnPlayer()
{
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUBSpawnPoint::StaticClass(), PlayerSpawnPoints);

    //스폰 위치가 겹치면 살짝 ㅇ옆으로 이동해서 스폰하는 언리얼 코드
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    if (PlayerClass.Num() == 0 || PlayerSpawnPoints.Num() == 0)
    {
        return;
    }
    for (int i = 0; i < PlayerClass.Num() && i < PlayerSpawnPoints.Num(); ++i)
    {
        const FTransform spawnTransform = PlayerSpawnPoints[i]->GetActorTransform();
        GetWorld()->SpawnActor<AUBPlayer>(PlayerClass[i], spawnTransform, Params);
    }
}


void AUBSpawnManager::SpawnMonster()
{
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUBMonsterSpawnPoint::StaticClass(), monsterSpawnPoints);
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    for (auto Actor : monsterSpawnPoints)
    {
        auto SP = Cast<AUBMonsterSpawnPoint>(Actor);
        for (auto MonsterClass : SP->MonsterClasses)
        {
            if (!MonsterClass) continue;
            auto Monster = GetWorld()->SpawnActor<AUBBaseMonster>(MonsterClass, SP->GetActorTransform(), Params);
        }
    }

}
