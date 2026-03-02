// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Common/Item/UBItem.h"
#include "Common/UBdefine.h"
#include "UBMaterial.generated.h"

/**
 * 
 */
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
	int32 itemID; //아이템 ID
	UPROPERTY(VisibleAnywhere)
	EEquipmentGrade equipmentGrade; //아이템 등급
	UPROPERTY(VisibleAnywhere)
	FString name; //이름
	UPROPERTY(VisibleAnywhere)
	FString icon; //아이콘
	UPROPERTY(VisibleAnywhere)
	int32 maxCount; //최대 소지 갯수
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 amount; //가지고 있는 갯수


};
