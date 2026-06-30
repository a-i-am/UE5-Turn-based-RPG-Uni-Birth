// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Common/UBdefine.h"
#include "UBItem.generated.h"

class UImage;


UINTERFACE(MinimalAPI)
class UUBItem : public UInterface
{
	GENERATED_BODY()
};




class UNIBIRTH_API IUBItem
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UTexture2D* GetIcon() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EItemType GetItemType() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EEquipmentGrade GetItemGrade() const;
};
