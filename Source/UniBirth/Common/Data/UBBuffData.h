// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Common/UBdefine.h"
#include "UBBuffData.generated.h"

USTRUCT(BlueprintType)
struct FBuffData : public FTableRowBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FSkillBuffData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString DelCase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EBuffDurType DurType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 TurnDurCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<EBuffStatType> BuffStats; // 여러 스탯 타입을 담는 배열

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EStatOperator StatOperator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<float> Values;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EUnitState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EBuffStackRule StackRule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 StackCount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EBuffType BuffType;
};


USTRUCT(BlueprintType)
struct FComboBuffData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EComboType Combo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString DelCase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EBuffDurType DurType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TurnDurationCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EBuffStatType BuffStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EStatOperator StatOperator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float LevelC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float LevelB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float LevelA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Icon_C;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Icon_B;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Icon_A;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EBuffType BuffType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

};
