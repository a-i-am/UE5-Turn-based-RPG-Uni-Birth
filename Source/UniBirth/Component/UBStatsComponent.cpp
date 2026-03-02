// Fill out your copyright notice in the Description page of Project Settings.


#include "UBStatsComponent.h"
#include "Common/System/UBGameInstance.h"
#include "Character/Interface/UBCombatUnit.h"
#include"Common/SubSystem/UBInventorySubsystem.h"

UUBStatsComponent::UUBStatsComponent()
{
}

// Called when the game starts
void UUBStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UUBStatsComponent::LogCurrentStats(const FString& Prefix)
{
    AActor* Owner = GetOwner();
    FString OwnerName = Owner ? Owner->GetName() : TEXT("None");

    FString Msg = FString::Printf(TEXT("[%s] %s"),
        *Prefix,
        *OwnerName
    );

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Magenta, Msg);

}

// Called every frame
void UUBStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUBStatsComponent::ApplyComboStatByType(EBuffStatType StatType, int32 Value, float Rate)
{
    FString EnumName = UEnum::GetValueAsString(StatType);
    UE_LOG(LogTemp, Warning, TEXT("ApplyComboStatByType Called - Type: %s, Value: %d, Rate: %f"), *EnumName, Value, Rate);

    switch (StatType)
    {
         case EBuffStatType::MaxHp: ApplyMaxHP(Value, Rate); break;
         case EBuffStatType::Hp: ApplyHPheal(Value, Rate); break;
         case EBuffStatType::Mp: ApplyMPHeal(Value, Rate); break;
         case EBuffStatType::Attack: ApplyAttackStats(Value, Rate); break;
         case EBuffStatType::CriticalRate: ApplyCritRate(Value, Rate); break;
         case EBuffStatType::StateResist: ApplyStateResist(Value, Rate); break;
         case EBuffStatType::Defence: ApplyDefence(Value, Rate); break;
         case EBuffStatType::Speed: ApplySpeedStats(Value, Rate); break;
         case EBuffStatType::Shield: ApplyShield(Value, Rate); break;
         case EBuffStatType::ParryPoint: ApplyParryPoint(Value, Rate); break;
         case EBuffStatType::DodgeTime: ApplyDodgeTime(Value, Rate); break;
         case EBuffStatType::AggroPoint: ApplyAggroPoint(Value, Rate); break;
    }
}

void UUBStatsComponent::ApplyMaxMP(int32 value, float rate)
{
    CalcStatValue(currentStats.MaxMp, stats.MaxMp, value, rate);

    if (currentStats.MaxMp < 1)
    {
        currentStats.MaxMp = 1;
    }

    currentStats.Mp = FMath::Clamp(currentStats.Mp, 0, currentStats.MaxMp);
    LogCurrentStats(TEXT("Apply MaxMp"));
}

void UUBStatsComponent::ApplyMPHeal(int32 value, float rate)
{
    int32 OldMp = currentStats.Mp;

    CalcStatValue(currentStats.Mp, stats.Mp, value, rate);
    
    if (currentStats.Mp < OldMp)
    {
        currentStats.Mp = OldMp;
    }

    currentStats.Mp = FMath::Clamp(currentStats.Mp, 0, currentStats.MaxMp);
    LogCurrentStats(TEXT("Apply MP"));
}

void UUBStatsComponent::ApplyAttackStats(int32 value, float rate)
{
    CalcStatValue(currentStats.Attack, stats.Attack, value, rate);
    LogCurrentStats(TEXT("Apply Attack"));
}

void UUBStatsComponent::ApplySpeedStats(int32 value, float rate)
{
    // 비율값으로 증가
    CalcStatValue(currentStats.AttackSpeed, stats.AttackSpeed, value, rate);
    LogCurrentStats(TEXT("Apply Speed"));

}
void UUBStatsComponent::ApplyMaxHP(int32 value, float rate)
{
    CalcStatValue(currentStats.MaxHP, stats.MaxHP, value, rate);

    if (currentStats.MaxHP < 1)
    {
        currentStats.MaxHP = 1;
    }

    currentStats.Hp = FMath::Clamp(currentStats.Hp, 0, currentStats.MaxHP);

    LogCurrentStats(TEXT("Apply MaxHP"));
}
void UUBStatsComponent::ApplyHPheal(int32 value, float rate)
{
    int32 OldHp = currentStats.Hp;
    
    // 머지 시 체력 버프가 해제되지 않고 추가되는 로직만 작동하니까 계산은 먼저 하기
    CalcStatValue(currentStats.Hp, stats.Hp, value, rate);
    
    // 버프가 해제될 때 결과적으로 기존 체력은 깎지 않게 처리
    if (currentStats.Hp < OldHp)
    {
        currentStats.Hp = OldHp;
    }
    
    currentStats.Hp = FMath::Clamp(currentStats.Hp, 0, currentStats.MaxHP);
    LogCurrentStats(TEXT("Apply HP"));
}
void UUBStatsComponent::ApplyDefence(int32 value, float rate)
{
    // 비율값으로 증가
    CalcStatValue(currentStats.Defence, stats.Defence, value, rate);
    LogCurrentStats(TEXT("Apply Defence"));
}
void UUBStatsComponent::ApplyShield(int32 value, float rate)
{
    CalcStatValue(currentStats.shield, stats.shield, value, rate);
    LogCurrentStats(TEXT("Apply Shield"));
}

void UUBStatsComponent::ApplyCritRate(int32 value, float rate)
{
    CalcStatValue(currentStats.critRate, stats.critRate, value, rate);
    LogCurrentStats(TEXT("Apply Critical Rate"));
}

void UUBStatsComponent::ApplyStateResist(int32 value, float rate)
{
    CalcStatValue(currentStats.ResistRate, stats.ResistRate, value, rate);
    LogCurrentStats(TEXT("Apply Critical Resist Rate"));
}

void UUBStatsComponent::ApplyAggroPoint(int32 value, float rate)
{
    CalcStatValue(currentStats.aggroPoint, stats.aggroPoint, value, rate);
    LogCurrentStats(TEXT("Apply AggroPoint"));
}

void UUBStatsComponent::ApplyParryPoint(int32 value, float rate)
{
    CalcStatValue(currentStats.parryPoint, stats.parryPoint, value, rate);
    LogCurrentStats(TEXT("Apply ParryPoint"));
}

void UUBStatsComponent::ApplyDodgeTime(int32 value, float rate)
{
    CalcStatValue<float>(currentStats.dodgeTime, stats.dodgeTime, value, rate);
    LogCurrentStats(TEXT("Apply DodgeTime"));
}

void UUBStatsComponent::InitStats(ECharacterType type)
{
    UUBGameInstance* gameinstance = Cast< UUBGameInstance>(GetOwner()->GetGameInstance());
    if (gameinstance == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("UUBGameInstance is nullptr"));
        return;
    }
    UUBInventorySubsystem* inventorysubsystem = UGameInstance::GetSubsystem<UUBInventorySubsystem>(GetWorld()->GetGameInstance());
    if (inventorysubsystem == nullptr)
        return;

    switch (type)
    {
    case ECharacterType::CT_Uni:
    case ECharacterType::CT_Tau:
    case ECharacterType::CT_Elvasia:
        stats = inventorysubsystem->GetEquippedStat(type);
        break;
    case ECharacterType::CT_Common:
    case ECharacterType::CT_Elite:
    case ECharacterType::CT_Boss:
        stats = gameinstance->GetStatData(type);
        break;
    }
    currentStats = stats;
    return;
}

void UUBStatsComponent::UpdateComboStat(EBuffStatType StatType, int32 NewFixed, float NewRate)
{
    int32* CurrFixed = ComboFixedBonusMap.Find(StatType);
    int32 OldFixed = CurrFixed ? *CurrFixed : 0;

    if (OldFixed != 0) 
    {
        ApplyComboStatByType(StatType, -OldFixed, 0.0f);
        ComboFixedBonusMap.FindOrAdd(StatType) = NewFixed;
    }

    if (NewFixed != 0)
    {
        ApplyComboStatByType(StatType, NewFixed, 0.0f);
    }

    float* CurrRate = ComboRateBonusMap.Find(StatType);
    float OldRate = CurrRate ? *CurrRate : 0.0f;

    if (OldRate != 0.0f)
    {
        ApplyComboStatByType(StatType, 0, -OldRate);
        ComboRateBonusMap.FindOrAdd(StatType) = NewRate;
    }
    if (NewRate != 0.0f)
    {
        ApplyComboStatByType(StatType, 0, NewRate);
    }
}