// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Main/UBMainWidget.h"
#include "UBCraftUI.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBCraftUI : public UUBMainWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UUBCraftSlot* CraftSlot_0;
	UPROPERTY(meta = (BindWidget))
	class UUBCraftSlot* CraftSlot_1;
	UPROPERTY(meta = (BindWidget))
	class UUBCraftSlot* CraftSlot_2;
	UPROPERTY(meta = (BindWidget))
	class UUBCraftSlot* CraftSlot_3;
	UPROPERTY(meta = (BindWidget))
	class UUBCraftSlot* CraftSlot_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray< UUBCraftSlot*> craftSlots;
	UFUNCTION(BlueprintCallable)
	void InitCraftList();

	UFUNCTION(BlueprintCallable)
	bool CheckItem(int index);
};
