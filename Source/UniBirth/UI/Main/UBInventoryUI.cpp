// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/UBInventoryUI.h"
#include "UI/Main/UBInventorySlot.h"
#include "Common/SubSystem/UBInventorySubsystem.h"
void UUBInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (UGameInstance* GI = GetGameInstance())
	{
		UUBInventorySubsystem* InventorySubsystem = GI->GetSubsystem<UUBInventorySubsystem>();
		if (InventorySubsystem)
		{
			InventorySubsystem->OnInventoryChanged.AddUObject(this, &UUBInventoryUI::ResetInventorySlots);
		}
	}

	ResetInventorySlots(); // ÃÖÃÊ 1È¸
}

void UUBInventoryUI::NativeDestruct()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		UUBInventorySubsystem* InventorySubsystem = GI->GetSubsystem<UUBInventorySubsystem>();
		if (InventorySubsystem)
		{
			InventorySubsystem->OnInventoryChanged
				.RemoveAll(this);
		}
	}

	Super::NativeDestruct();
}

void UUBInventoryUI::InitInventoryList()
{

	inventorySlots.Add(InventorySlot_0);
	inventorySlots.Add(InventorySlot_1);
	inventorySlots.Add(InventorySlot_2);
	inventorySlots.Add(InventorySlot_3);
	inventorySlots.Add(InventorySlot_4);
	inventorySlots.Add(InventorySlot_5);
	inventorySlots.Add(InventorySlot_6);
	inventorySlots.Add(InventorySlot_7);
	inventorySlots.Add(InventorySlot_8);
	inventorySlots.Add(InventorySlot_9);
	inventorySlots.Add(InventorySlot_10);
	inventorySlots.Add(InventorySlot_11);
	inventorySlots.Add(InventorySlot_12);
	inventorySlots.Add(InventorySlot_13);
	inventorySlots.Add(InventorySlot_14);
	inventorySlots.Add(InventorySlot_15);
	inventorySlots.Add(InventorySlot_16);
	inventorySlots.Add(InventorySlot_17);
	inventorySlots.Add(InventorySlot_18);
	inventorySlots.Add(InventorySlot_19);
	inventorySlots.Add(InventorySlot_20);
	inventorySlots.Add(InventorySlot_21);
	inventorySlots.Add(InventorySlot_22);
	inventorySlots.Add(InventorySlot_23);
	inventorySlots.Add(InventorySlot_24);
}

void UUBInventoryUI::ResetInventorySlots()
{
	for (auto slot : inventorySlots) {
		slot->InitSlot();
	}
}
