// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "UBInventoryDrag.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBInventoryDrag : public UDragDropOperation
{
	GENERATED_BODY()
public:
	//인벤토리 인덱스
	UPROPERTY(BlueprintReadWrite)
	int32 index;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* icon;
	
};
