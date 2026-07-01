// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Common/UBdefine.h"
#include "Common/Item/UBItem.h"
#include "UBEquipment.generated.h"




UCLASS(BlueprintType)
class UNIBIRTH_API UUBEquipment : public UObject, public IUBItem
{
	GENERATED_BODY()


public:
	UFUNCTION()
	UTexture2D* GetIcon_Implementation() const;

	virtual EItemType GetItemType_Implementation() const override;
	virtual EEquipmentGrade GetItemGrade_Implementation() const override;
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	EEquipmentType equipmentType;
	UPROPERTY(VisibleAnywhere)
	EEquipmentGrade equipmentGrade;
	UPROPERTY(VisibleAnywhere)
	int statvalue;
	UPROPERTY(VisibleAnywhere)
	FString name;
	UPROPERTY(VisibleAnywhere)
	int32 itemID;
	UPROPERTY(VisibleAnywhere)
	FString icon;

	UFUNCTION()
	void Equip();

	UFUNCTION()
	void UnEquip();
};
