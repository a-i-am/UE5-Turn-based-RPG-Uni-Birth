// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Common/UBdefine.h"
#include "UBPlayerWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNIBIRTH_API UUBPlayerWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EBattleUIType uiType;
};
