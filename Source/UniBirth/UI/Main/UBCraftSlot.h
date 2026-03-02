// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Main/UBMainWidget.h"
#include "UBCraftSlot.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBCraftSlot : public UUBMainWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 slotindex;

	UFUNCTION(BlueprintCallable)
	void InitSlot();

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UImage* itemicon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUBCraftUI* craftUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<class IUBItem> item;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
};
