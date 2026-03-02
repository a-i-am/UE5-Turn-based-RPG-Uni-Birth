// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/UBCraftUI.h"
#include"Common/SubSystem/UBInventorySubsystem.h"
#include "Common/Item/UBMaterial.h"
#include "Common/Item/UBItem.h"
#include "Common/UBdefine.h"
#include "Common/Item/UBEquipment.h"
#include "Common/Item/UBMaterial.h"
#include "UI/Main/UBCraftSlot.h"

void UUBCraftUI::InitCraftList()
{
	craftSlots.Add(CraftSlot_0);
	craftSlots.Add(CraftSlot_1);
	craftSlots.Add(CraftSlot_2);
	craftSlots.Add(CraftSlot_3);
	craftSlots.Add(CraftSlot_4);

	CraftSlot_0->craftUI = this;
	CraftSlot_1->craftUI = this;
	CraftSlot_2->craftUI = this;
	CraftSlot_3->craftUI = this;
	CraftSlot_4->craftUI = this;
}

bool UUBCraftUI::CheckItem(int index)
{
	if (GetWorld()==nullptr) {
		return false;
	}
	UUBInventorySubsystem* inventorysubsystem = UGameInstance::GetSubsystem<UUBInventorySubsystem>(GetWorld()->GetGameInstance());
	if (inventorysubsystem == nullptr)
		return false;
	TScriptInterface<IUBItem>* tempitem = inventorysubsystem->inventory.Find(index);
	if (tempitem ==nullptr) {
		return false;
	}
	//등급체크
	EEquipmentGrade itemgrade = IUBItem::Execute_GetItemGrade(tempitem->GetObject());
	for (auto slot : craftSlots) {
		if(slot->item==nullptr)
		{
			continue;
		}
		if (IUBItem::Execute_GetItemGrade(slot->item.GetObject()) != itemgrade)
		{
			return false;
		}
	}
	//만약 장비조각이라면
	EItemType itemtype = IUBItem::Execute_GetItemType(tempitem->GetObject());
	if (itemtype == EItemType::IT_Material) {
		UUBMaterial* material = Cast<UUBMaterial>(tempitem->GetObject());
		if (material == nullptr) {
			return false;
		}
		int count = 0;
		int amount = material->amount;
		for (auto slot : craftSlots) {
			if (slot->item == nullptr)
			{
				continue;
			}
			if (IUBItem::Execute_GetItemType(slot->item.GetObject()) == EItemType::IT_Material)
			{
				count++;
			}
		}
		if (count >= amount)
		{
			return false;
		}
	}
	else if (itemtype == EItemType::IT_Equipment) {
		UUBEquipment* equipment = Cast<UUBEquipment>(tempitem->GetObject());
		if (equipment == nullptr) {
			return false;
		}
		for (auto slot : craftSlots) {
			if (slot->item == nullptr)
			{
				continue;
			}
			if (slot->slotindex == index)
			{
				return false;
			}
		}
	}
	return true;
}
