// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UBMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void ShowWidget();
	UFUNCTION(BlueprintCallable)
	void HideWidget();
	UFUNCTION(BlueprintCallable)
	void HoverWidget();
	void Tick(FGeometry MyGeometry, float InDeltaTime);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//누르면 나오는 위젯
	TSubclassOf<UUserWidget> TargetWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//누르면 나오는 위젯
	TSubclassOf<UUserWidget> TransparentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* CreatedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUBTransparentUI* CreatedTransparentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUBMainWidget* OwnerWidget;
};
