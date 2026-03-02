// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/UBInventorySlot.h"
#include"Common/SubSystem/UBInventorySubsystem.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Main/UBInventoryDrag.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UUBInventorySlot::InitSlot() {
	//인벤토리에서 인덱스에 있는 아이템 불러와서 아이콘 등록하기
	if (itemicon == nullptr)
		return;
	UUBInventorySubsystem* inventorysubsystem = UGameInstance::GetSubsystem<UUBInventorySubsystem>(GetWorld()->GetGameInstance());
	if (inventorysubsystem == nullptr)
		return;
	TScriptInterface<IUBItem>* tempitem = inventorysubsystem->inventory.Find(slotindex);
	if (tempitem == nullptr) {
		itemicon->SetBrushFromTexture(nullptr);
		item = nullptr;
		return;
	}
	else {
		item = *tempitem;
	}
	

	UTexture2D* icon = IUBItem::Execute_GetIcon(item.GetObject());
	if (icon == nullptr)
	{
		itemicon->SetBrushFromTexture(nullptr);
		return;
	}
	
	itemicon->SetBrushFromTexture(icon);
}

FReply UUBInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 좌클릭 입력이 들어온 경우
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (item.GetObject() !=nullptr)
		{
			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
	}

	return Reply.NativeReply;
}

void UUBInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		// 드래그 슬롯을 생성합니다.
		UUBInventoryDrag* Operation = NewObject<UUBInventoryDrag>();
		OutOperation = Operation;

		// 슬롯과 슬롯 타입을 지정합니다.
		Operation->index = slotindex;
		Operation->icon = IUBItem::Execute_GetIcon(item.GetObject());

		// Drag 위젯을 생성합니다.
		if (DragWidgetClass)
		{
			UUBInventorySlot* DragWidget = CreateWidget<UUBInventorySlot>(GetWorld(), DragWidgetClass);
			if (DragWidget)
			{
				// 생성된 위젯을 초기화해줍니다.
				DragWidget->slotindex = slotindex;
				DragWidget->InitSlot();

				// 드래그 슬롯의 드래그 위젯을 설정합니다.
				Operation->DefaultDragVisual = DragWidget;
			}
		}
	}
}

bool UUBInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UUBInventoryDrag* Operation = Cast<UUBInventoryDrag>(InOperation);

	if (Operation)
	{
		UUBInventorySubsystem* inventorysubsystem = UGameInstance::GetSubsystem<UUBInventorySubsystem>(GetWorld()->GetGameInstance());
		if (inventorysubsystem == nullptr)
			return false;
		inventorysubsystem->SwapIndex(Operation->index, slotindex);
		return true;

	}
	return false;
}

void UUBInventorySlot::ShowWidget() {
	Super::ShowWidget();


	if (!CreatedWidget || !CreatedWidget->IsInViewport())
		return;

	FVector2D MousePos;
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetWorld()->GetFirstPlayerController(), MousePos.X, MousePos.Y);
	CreatedWidget->SetAlignmentInViewport(FVector2D(0.f, 0.f));
	CreatedWidget->SetPositionInViewport(MousePos, false);
}
