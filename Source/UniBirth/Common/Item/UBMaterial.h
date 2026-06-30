// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Common/Item/UBItem.h"
#include "Common/UBdefine.h"
#include "UBMaterial.generated.h"




UCLASS(BlueprintType)
class UNIBIRTH_API UUBMaterial : public UObject, public IUBItem
{
	GENERATED_BODY()
public:
	UFUNCTION()
	UTexture2D* GetIcon_Implementation() const;
	virtual EItemType GetItemType_Implementation() const override;
	virtual EEquipmentGrade GetItemGrade_Implementation() const override;
public:
	UPROPERTY(VisibleAnywhere)
	int32 itemID;
	UPROPERTY(VisibleAnywhere)
	EEquipmentGrade equipmentGrade;
	UPROPERTY(VisibleAnywhere)
	FString name;
	UPROPERTY(VisibleAnywhere)
	FString icon;
	UPROPERTY(VisibleAnywhere)
	int32 maxCount;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 amount;


};
