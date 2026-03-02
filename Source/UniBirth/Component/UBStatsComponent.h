// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include  "Character/Interface/FUBStats.h"
#include "UBStatsComponent.generated.h"
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDodgeStatChanged, int32 NewValue);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNIBIRTH_API UUBStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUBStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void LogCurrentStats(const FString& Prefix);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ApplyComboStatByType(EBuffStatType StatType, int32 Value, float Rate);
	
	void ApplyHPheal(int32 value, float rate);
	void ApplyMaxMP(int32 value, float rate);
	void ApplyMPHeal(int32 value, float rate);
	void ApplyMaxHP(int32 value, float rate);
	void ApplyAttackStats(int32 value, float rate);
	void ApplySpeedStats(int32 value, float rate);
	void ApplyDefence(int32 value, float rate);
	void ApplyShield(int32 value, float rate);
	void ApplyCritRate(int32 value, float rate);
	void ApplyStateResist(int32 value, float rate);
	void ApplyAggroPoint(int32 value, float rate);
	void ApplyParryPoint(int32 value, float rate);
	void ApplyDodgeTime(int32 value, float rate);

	UFUNCTION(BlueprintCallable)
	void InitStats(ECharacterType type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FUBStats stats;	// ÃÊ±â ½ºÅÈ°ª

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FUBStats currentStats;

	void UpdateComboStat(EBuffStatType StatType, int32 NewFixed, float NewRate);
private:
	float CurrentComboRateBonus = 0.0f;
	TMap<EBuffStatType, float> ComboRateBonusMap;
	TMap<EBuffStatType, int32> ComboFixedBonusMap;

	template<class T>
	void CalcStatValue(T& targetStatValue, int32 sourceStatValue, int32 value, float rate)
	{
		if (rate != 0)
		{
			targetStatValue += (sourceStatValue * rate);
		}
		else
		{
			targetStatValue += value;
		}
	}
	

public:
	FOnDodgeStatChanged OnAddDodgeStat;
};