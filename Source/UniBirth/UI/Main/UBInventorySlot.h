// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Main/UBMainWidget.h"
#include "Common/Item/UBItem.h"
#include "UBInventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBInventorySlot : public UUBMainWidget
{
	GENERATED_BODY()
public:
	void ShowWidget() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 slotindex;

	UFUNCTION(BlueprintCallable)
	void InitSlot();

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UImage* itemicon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<class IUBItem> item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf< UUBInventorySlot> DragWidgetClass;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
};
