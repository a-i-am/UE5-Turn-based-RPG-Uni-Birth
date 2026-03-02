// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/UBdefine.h"
#include "Component/UBBuffComponent.h"
#include "Component/UBComboBuffComponent.h"
#include "Character/Interface/UBCombatUnit.h"
#include "UBBattleCharacterHP.generated.h"


UCLASS()
class UNIBIRTH_API UUBBattleCharacterHP : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;// 위젯이 생성될 때 초기화하는 함수
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UUniformGridPanel* buffImageGrid;

	UPROPERTY(meta = (BindWidget))
	TArray<class UImage*> buffIcons;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HP_Progress;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* MP_Progress;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Shield_Progress;

	UPROPERTY(BlueprintReadWrite)
	class UMaterialInstanceDynamic* grayscaleMID;
public:
	void UpdateHPBar(float currentHP, float maxHP);
	void UpdateMPBar(float currentMP, float maxMP);
	void UpdateShieldBar(float currentShield, float maxShield);
	UFUNCTION(BlueprintImplementableEvent)
	void SetGrayscale();

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
