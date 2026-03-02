// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UBTimeProgressWidget.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnTimeEXpired);
class UImage;
UCLASS(BlueprintType, Blueprintable)
class UNIBIRTH_API UUBTimeProgressWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta = (BindWidget))
	UImage* timeProgressImg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category =  "Progress", meta = (DisplayName = "프로그래스바 활성화"))
	bool bIsProgressRunning = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progress", meta = (DisplayName = "총 제한시간"))
	float defaultMaxTime = 5.0f;

	void SetMaxTime(float buffTime);

public:
	float currentTime = 0.0f;
	float maxTime = 5.0f;
	bool bTimeOut= false;

	float GetCurrentTime() const { return currentTime; }
	FOnTimeEXpired OnTimeEXpried;
	
};
