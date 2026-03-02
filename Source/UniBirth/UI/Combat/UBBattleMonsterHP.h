// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/UBdefine.h"
#include "Component/UBBuffComponent.h"
#include "Component/UBComboBuffComponent.h"
#include "Character/Interface/UBCombatUnit.h"
#include "UBBattleMonsterHP.generated.h"

UCLASS()
class UNIBIRTH_API UUBBattleMonsterHP : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;// 위젯이 생성될 때 초기화하는 함수
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MonsterName;

	UPROPERTY(blueprintReadWrite, meta = (BindWidget))
	class UUniformGridPanel* buffImageGrid;

	UPROPERTY(meta = (BindWidget))
	TArray<class UImage*> buffIcons;

	UPROPERTY(blueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HP_Progress;

	UPROPERTY(blueprintReadWrite, meta = (BindWidget))
	class UProgressBar* MP_Progress;

	UPROPERTY(blueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Shield_Progress;
public:
	void UpdateHPBar(float currentHP, float maxHP);
	void UpdateMPBar(float currentMP, float maxMP);
	void UpdateShieldBar(float currentShield, float maxShield);
	UFUNCTION(BlueprintCallable)
	void UpdateBuffIcons();
public:
	UPROPERTY(BlueprintReadWrite)
	AUBCombatUnit* currCharacter;

	int32 currentHP = 0;
	int32 currentMP = 0;
	int32 currentShield = 0;

	int32 maxHP = 0;
	int32 maxMP = 0;
	int32 maxShield = 1700;
	
	UPROPERTY()
	TObjectPtr<class ABattleManager> BM;
};