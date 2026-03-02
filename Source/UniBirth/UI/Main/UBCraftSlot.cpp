// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/UBCraftSlot.h"
#include"Common/SubSystem/UBInventorySubsystem.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Main/UBInventoryDrag.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/Main/UBInventorySlot.h"
#include "UI/Main/UBCraftUI.h"

void UUBCraftSlot::InitSlot() {
	//인벤토리에서 인덱스에 있는 아이템 불러와서 아이콘 등록하기
	if (item == nullptr)
		return;
	UTexture2D* icon = IUBItem::Execute_GetIcon(item.GetObject());
	if (icon == nullptr)
	{
		itemicon->SetBrushFromTexture(nullptr);
		return;
	}

	itemicon->SetBrushFromTexture(icon);
}


bool UUBCraftSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UUBInventoryDrag* Operation = Cast<UUBInventoryDrag>(InOperation);

	if (Operation)
	{
		UUBInventorySubsystem* inventorysubsystem = UGameInstance::GetSubsystem<UUBInventorySubsystem>(GetWorld()->GetGameInstance());
		if (inventorysubsystem == nullptr)
			return false;
		if (inventorysubsystem->inventory[Operation->index] == nullptr)
		{
			return false;
		}
		//등급, 아이템 갯수 확인
		if (!craftUI->CheckItem(Operation->index)) {
			return false;
		}
		item = inventorysubsystem->inventory[Operation->index].GetObject();
		slotindex = Operation->index;
		InitSlot();
	}
	return false;
}

