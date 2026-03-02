// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UBTransparentUI.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBTransparentUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* OwnerWidget;
};
