// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Main/UBMainWidget.h"
#include "UBInventoryUI.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBInventoryUI : public UUBMainWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void NativeConstruct() override;
	UFUNCTION()
	void NativeDestruct() override;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_0;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_1;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_2;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_3;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_4;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_5;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_6;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_7;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_8;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_9;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_10;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_11;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_12;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_13;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_14;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_15;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_16;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_17;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_18;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_19;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_20;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_21;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_22;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_23;
	UPROPERTY(meta = (BindWidget))
	class UUBInventorySlot* InventorySlot_24;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray< UUBInventorySlot*> inventorySlots;
	UFUNCTION(BlueprintCallable)
	void InitInventoryList();

	UFUNCTION(BlueprintCallable)
	void ResetInventorySlots();
};
