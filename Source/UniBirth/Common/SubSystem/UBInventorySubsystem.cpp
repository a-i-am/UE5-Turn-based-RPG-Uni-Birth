// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SubSystem/UBInventorySubsystem.h"
#include "Common/System/UBGameInstance.h"
#include "Common/Item/UBArmor.h"
#include "Common/Item/UBConsumable.h"
#include "Common/Item/UBWeapon.h"
#include "Common/Item/UBEquipment.h"
#include "Common/Data/UBSaveGame.h"
#include "Common/Item/UBMaterial.h"
#include "Kismet/GameplayStatics.h"

void UUBInventorySubsystem::SaveInventory()
{
	if (UUBSaveGame* LoadedGame = Cast<UUBSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainData"), 0)))
	{
		SavedInventory.Empty();
		
		for (auto& item : inventory)
		{
			if (!item.Value)
			{
				continue;
			}
			if (item.Value.GetObject()->GetClass()->ImplementsInterface(UUBItem::StaticClass()))
			{
				EItemType ItemType = IUBItem::Execute_GetItemType(item.Value.GetObject());
				FInventoryStruct invenstruct;

				if (ItemType == EItemType::IT_Consumable)
				{
					UUBConsumable* consumable = Cast<UUBConsumable>(item.Value.GetObject());
					invenstruct.itemID = consumable->itemID;
					invenstruct.amount = consumable->amount;
					invenstruct.itemtype = EItemType::IT_Consumable;
					invenstruct.bullettype = EBulletType::BT_Normal;
					invenstruct.slotindex = item.Key;
					SavedInventory.Add(invenstruct);
				}
				else if (ItemType == EItemType::IT_Equipment)
				{
					UUBEquipment* equipment = Cast<UUBEquipment>(item.Value.GetObject());
					invenstruct.itemID = equipment->itemID;
					invenstruct.amount = 1;
					invenstruct.itemtype = EItemType::IT_Equipment;
					if (equipment->equipmentType == EEquipmentType::ET_Weapon) {
						if (UUBWeapon* weapon = Cast< UUBWeapon>(equipment))
						{
							invenstruct.bullettype=weapon->bullettype;
						}
					}
					invenstruct.slotindex = item.Key;
					SavedInventory.Add(invenstruct);

				}
				else if (ItemType == EItemType::IT_Material)
				{
					UUBMaterial* material = Cast<UUBMaterial>(item.Value.GetObject());
					invenstruct.itemID = material->itemID;
					invenstruct.amount = material->amount;
					invenstruct.itemtype = EItemType::IT_Material;
					invenstruct.bullettype = EBulletType::BT_Normal;
					invenstruct.slotindex = item.Key;
					SavedInventory.Add(invenstruct);
				}
			}
		}

		LoadedGame->Inventory = SavedInventory;
		UGameplayStatics::SaveGameToSlot(LoadedGame, TEXT("MainData"), 0);
	}
}

void UUBInventorySubsystem::LoadInventory()
{
	if (UUBSaveGame* LoadedGame = Cast<UUBSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainData"), 0)))
	{
		TArray<FInventoryStruct> savedInventory =LoadedGame->Inventory;

		for (auto& item : savedInventory)
		{
			AddItem(item.itemtype, item.itemID, item.amount,item.slotindex, item.bullettype);
		}
	}
}

bool UUBInventorySubsystem::AddItem(EItemType itemtype, int32 itemID,int32 amount ,int32 invenindex , EBulletType bullettype)
{

	UUBGameInstance* gameinstance = Cast< UUBGameInstance>(GetGameInstance());
	if (gameinstance == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("UUBGameInstance is nullptr"));
		return false;
	}

	switch (itemtype) {
	case EItemType::IT_Consumable:
	{
		int32 index = GetConsumablesInInventory(itemID);
		UE_LOG(LogTemp, Warning, TEXT("index = %d"), index);
		if (index != -1)
		{
			if (UUBConsumable* item = Cast<UUBConsumable>(inventory[index].GetObject()))
			{
				item->amount++;
				SaveInventory();
				return true;
			}
		}
		else {
			UUBConsumable* consumable = GetConsumablesByID(itemID);
			consumable->amount = amount;
			if (invenindex != -1) {
				inventory.Add(invenindex, consumable);
			}
			else {
				if (inventory.Num() >= INVENTORY_SIZE) {
					UE_LOG(LogTemp, Warning, TEXT("Inventory is already full"));
					return false;
				}
				SortInventory();
				inventory.Add(inventory.Num() , consumable);
			}
			SaveInventory();
			return true;
		}
	}return false;
		
	case EItemType::IT_Equipment:
	{
		if (inventory.Num() >= INVENTORY_SIZE) {
			UE_LOG(LogTemp, Warning, TEXT("Inventory is already full"));
			return false;
		}
		UUBEquipment* equipment = GetEquipmentsByID(itemID);

		if (UUBWeapon* weapon = Cast<UUBWeapon>(equipment)) {
			weapon->bullettype = bullettype;
			if (invenindex != -1) {
				inventory.Add(invenindex, weapon);
			}
			else {
				if (inventory.Num() >= INVENTORY_SIZE) {
					UE_LOG(LogTemp, Warning, TEXT("Inventory is already full"));
					return false;
				}
				SortInventory();
				inventory.Add(inventory.Num(), weapon);
			}

			SaveInventory();
			return true;
		}
		else if (UUBArmor* armor = Cast<UUBArmor>(equipment))
		{
			if (invenindex != -1) {
				inventory.Add(invenindex, armor);
			}
			else {
				if (inventory.Num() >= INVENTORY_SIZE) {
					UE_LOG(LogTemp, Warning, TEXT("Inventory is already full"));
					return false;
				}
				SortInventory();
				inventory.Add(inventory.Num(), armor);
			}
			SaveInventory();
			return true;
		}
	}return false;
	case EItemType::IT_Material:
	{
		int32 index = GetMaterialsInInventory(itemID);
		UE_LOG(LogTemp, Warning, TEXT("index = %d"), index);
		if (index != -1)
		{
			if (UUBMaterial* item = Cast<UUBMaterial>(inventory[index].GetObject()))
			{
				item->amount += amount;
				SaveInventory();
				return true;
			}
		}
		else {
			UUBMaterial* material = GetMaterialsByID(itemID);
			if (material == nullptr)
				return false;
			material->amount = amount;
			if (invenindex != -1) {
				inventory.Add(invenindex, material);
			}
			else {
				if (inventory.Num() >= INVENTORY_SIZE) {
					UE_LOG(LogTemp, Warning, TEXT("Inventory is already full"));
					return false;
				}
				SortInventory();
				inventory.Add(inventory.Num(), material);
			}
			SaveInventory();
			return true;
		}
	}return false;
	}
	return false;
}

bool UUBInventorySubsystem::RemoveItem(int32 slotindex)
{
	if (!inventory.Contains(slotindex))
		return false;
	if (IUBItem::Execute_GetItemType(inventory[slotindex].GetObject()) == EItemType::IT_Material) {
		UUBMaterial* material = Cast<UUBMaterial>(inventory[slotindex].GetObject());
		if (material) {
			material->amount--;
			if (material->amount >= 1) {
				return true;
			}
		}
	}
	inventory.Remove(slotindex);
	SortInventory();
	SaveInventory();
	return true;
}

int32 UUBInventorySubsystem::GetConsumablesInInventory(int32 itemid) {

	for (auto& item : inventory)
	{
		UUBConsumable* consumable = Cast<UUBConsumable>(item.Value.GetObject());
		if (consumable == nullptr)
			continue;
		if (consumable->itemID == itemid)
		{
			return item.Key;
		}
	}
	return -1;
}

int32 UUBInventorySubsystem::GetMaterialsInInventory(int32 itemid) {

	for (auto& item : inventory)
	{
		UUBMaterial* material = Cast<UUBMaterial>(item.Value.GetObject());
		if (material == nullptr)
			continue;
		if (material->itemID == itemid)
		{
			return item.Key;
		}
	}
	return -1;
}

UUBConsumable* UUBInventorySubsystem::GetConsumablesByID(int32 itemid)
{
	UUBGameInstance* gameinstance = Cast< UUBGameInstance>(GetGameInstance());
	if (gameinstance == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("UUBGameInstance is nullptr"));
		return nullptr;
	}

	if (FConsumableStruct* consumabledata = gameinstance->GetConsumableData(itemid))
	{
		UUBConsumable* consumable = NewObject<UUBConsumable>(this);
		//데이터 쭉 적용
		consumable->maxCount = consumabledata->maxCount;
		consumable->maxUseCount = consumabledata->maxUseCount;
		consumable->name = consumabledata->Name;
		consumable->icon = consumabledata->Icon;
		consumable->itemID = itemid;
		return consumable;
	}
	UE_LOG(LogTemp, Warning, TEXT("Consumabledata is nullptr"));
	return nullptr;
}

UUBEquipment* UUBInventorySubsystem::GetEquipmentsByID(int32 itemid)
{
	UUBGameInstance* gameinstance = Cast< UUBGameInstance>(GetGameInstance());
	if (gameinstance == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("UUBGameInstance is nullptr"));
		return nullptr;
	}

	if (FEquipmentStruct* equipmentdata = gameinstance->GetEquipmentData(itemid))
	{
		if (equipmentdata->EquipmentType == EEquipmentType::ET_Weapon)
		{
			UUBWeapon* weapon = NewObject<UUBWeapon>(this);
			
			weapon->weapontype = equipmentdata->Weapontype;
			weapon->statvalue = equipmentdata->Value;
			weapon->equipmentType = equipmentdata->EquipmentType;
			weapon->equipmentGrade = equipmentdata->EquipmentGrade;
			weapon->name = equipmentdata->Name;
			weapon->icon = equipmentdata->Icon;
			weapon->itemID = itemid;
			
			return weapon;
		}
		else {
			UUBArmor* armor = NewObject<UUBArmor>(this);

			armor->statvalue = equipmentdata->Value;
			armor->equipmentType = equipmentdata->EquipmentType;
			armor->equipmentGrade = equipmentdata->EquipmentGrade;
			armor->name = equipmentdata->Name;
			armor->icon = equipmentdata->Icon;
			armor->itemID = itemid;
			
			return armor;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Equipmentdata is nullptr"));
	return nullptr;
}

UUBMaterial* UUBInventorySubsystem::GetMaterialsByID(int32 itemid)
{
	UUBGameInstance* gameinstance = Cast< UUBGameInstance>(GetGameInstance());
	if (gameinstance == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("UUBGameInstance is nullptr"));
		return nullptr;
	}

	if (FMaterialStruct* materialdata = gameinstance->GetMaterialData(itemid))
	{
		UUBMaterial* material = NewObject<UUBMaterial>(this);
		//데이터 쭉 적용
		material->maxCount = materialdata->Amount;
		material->name = materialdata->Name;
		material->equipmentGrade = materialdata->EquipmentGrade;
		material->icon = materialdata->Icon;
		material->itemID = itemid;
		return material;
	}
	UE_LOG(LogTemp, Warning, TEXT("Consumabledata is nullptr"));
	return nullptr;
}

void UUBInventorySubsystem::SaveEquipment() {
	if (UUBSaveGame* LoadedGame = Cast<UUBSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainData"), 0)))
	{
		FCharacterEquipmentSaveStruct uni = LoadedGame->UniEquipment;
		if (uniEquipment.Weapon) {
			uni.WeaponID = uniEquipment.Weapon->itemID;
		}
		if (uniEquipment.Head) {
			uni.HeadID = uniEquipment.Head->itemID;
		}
		if (uniEquipment.Chest) {
			uni.ChestID = uniEquipment.Chest->itemID;
		}
		if (uniEquipment.Hand) {
			uni.HandID = uniEquipment.Hand->itemID;
		}
		if (uniEquipment.Drone) {
			uni.DroneID = uniEquipment.Drone->itemID;
		}
		

		FCharacterEquipmentSaveStruct tau = LoadedGame->TauEquipment;
		if (tauEquipment.Weapon) {
			tau.WeaponID = tauEquipment.Weapon->itemID;
		}
		if (tauEquipment.Head) {
			tau.HeadID = tauEquipment.Head->itemID;
		}
		if (tauEquipment.Chest) {
			tau.ChestID = tauEquipment.Chest->itemID;
		}
		if (tauEquipment.Hand) {
			tau.HandID = tauEquipment.Hand->itemID;
		}
		if (tauEquipment.Drone) {
			tau.DroneID = tauEquipment.Drone->itemID;
		}

		FCharacterEquipmentSaveStruct elvasia = LoadedGame->ElvasiaEquipment;
		if (elvasiaEquipment.Weapon) {
			elvasia.WeaponID = elvasiaEquipment.Weapon->itemID;
		}
		if (elvasiaEquipment.Head) {
			elvasia.HeadID = elvasiaEquipment.Head->itemID;
		}
		if (elvasiaEquipment.Chest) {
			elvasia.ChestID = elvasiaEquipment.Chest->itemID;
		}
		if (elvasiaEquipment.Hand) {
			elvasia.HandID = elvasiaEquipment.Hand->itemID;
		}
		if (elvasiaEquipment.Drone) {
			elvasia.DroneID = elvasiaEquipment.Drone->itemID;
		}

		LoadedGame->UniEquipment = uni;
		LoadedGame->TauEquipment = tau;
		LoadedGame->ElvasiaEquipment = elvasia;
		UGameplayStatics::SaveGameToSlot(LoadedGame, TEXT("MainData"), 0);
	}
}

void UUBInventorySubsystem::LoadEquipment() {
	if (UUBSaveGame* LoadedGame = Cast<UUBSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainData"), 0)))
	{
		FCharacterEquipmentSaveStruct uni = LoadedGame->UniEquipment;
		FCharacterEquipmentSaveStruct tau = LoadedGame->TauEquipment;
		FCharacterEquipmentSaveStruct elvasia = LoadedGame->ElvasiaEquipment;

		uniEquipment.Weapon = Cast<UUBWeapon>(GetEquipmentsByID(uni.WeaponID));
		uniEquipment.Head = Cast<UUBArmor>(GetEquipmentsByID(uni.HeadID));
		uniEquipment.Chest = Cast<UUBArmor>(GetEquipmentsByID(uni.ChestID));
		uniEquipment.Hand = Cast<UUBArmor>(GetEquipmentsByID(uni.HandID));
		uniEquipment.Drone = Cast<UUBArmor>(GetEquipmentsByID(uni.DroneID));

		tauEquipment.Weapon = Cast<UUBWeapon>(GetEquipmentsByID(tau.WeaponID));
		tauEquipment.Head = Cast<UUBArmor>(GetEquipmentsByID(tau.HeadID));
		tauEquipment.Chest = Cast<UUBArmor>(GetEquipmentsByID(tau.ChestID));
		tauEquipment.Hand = Cast<UUBArmor>(GetEquipmentsByID(tau.HandID));
		tauEquipment.Drone = Cast<UUBArmor>(GetEquipmentsByID(tau.DroneID));

		elvasiaEquipment.Weapon = Cast<UUBWeapon>(GetEquipmentsByID(elvasia.WeaponID));
		elvasiaEquipment.Head = Cast<UUBArmor>(GetEquipmentsByID(elvasia.HeadID));
		elvasiaEquipment.Chest = Cast<UUBArmor>(GetEquipmentsByID(elvasia.ChestID));
		elvasiaEquipment.Hand = Cast<UUBArmor>(GetEquipmentsByID(elvasia.HandID));
		elvasiaEquipment.Drone = Cast<UUBArmor>(GetEquipmentsByID(elvasia.DroneID));
	}
}

void UUBInventorySubsystem::Equip(UUBEquipment* equipment, ECharacterType character) {
#pragma region swtichcase
	switch (equipment->equipmentType)
	{
	case EEquipmentType::ET_Drone:
		switch (character)
		{
		case ECharacterType::CT_Uni:
			if (UUBArmor* armor = Cast<UUBArmor>(equipment)) {
				uniEquipment.Drone = armor;
			}
			break;
		case ECharacterType::CT_Tau:
			if (UUBArmor* armor = Cast<UUBArmor>(equipment)) {
				tauEquipment.Drone = armor;
			}
			break;
		case ECharacterType::CT_Elvasia:
			if (UUBArmor* armor = Cast<UUBArmor>(equipment)) {
				elvasiaEquipment.Drone = armor;
			}
			break;
		default:
			break;
		}
		break;
	case EEquipmentType::ET_Weapon:
		switch (character)
		{
		case ECharacterType::CT_Uni:
			if (UUBWeapon* weapon = Cast<UUBWeapon>(equipment)) {
				if (weapon->weapontype != ECharacterType::CT_Uni) {
					break;
				}
				uniEquipment.Weapon = weapon;
			}
			break;
		case ECharacterType::CT_Tau:
			if (UUBWeapon* weapon = Cast<UUBWeapon>(equipment)) {
				if (weapon->weapontype != ECharacterType::CT_Tau) {
					break;
				}
				tauEquipment.Weapon = weapon;
			}
			break;
		case ECharacterType::CT_Elvasia:
			if (UUBWeapon* weapon = Cast<UUBWeapon>(equipment)) {
				if (weapon->weapontype != ECharacterType::CT_Elvasia) {
					break;
				}
				elvasiaEquipment.Weapon = weapon;
			}
			break;
		default:
			break;
		}
		break;
	case EEquipmentType::ET_Head:
		switch (character)
		{
		case ECharacterType::CT_Uni:
			if (UUBArmor* armor = Cast<UUBArmor>(equipment)) {
				uniEquipment.Head = armor;
			}
			break;
		case ECharacterType::CT_Tau:
			if (UUBArmor* armor = Cast<UUBArmor>(equipment)) {
				tauEquipment.Head = armor;
			}
			break;
		case ECharacterType::CT_Elvasia:
			if (UUBArmor* armor = Cast<UUBArmor>(equipment)) {
				elvasiaEquipment.Head = armor;
			}
			break;
		default:
			break;
		}
		break;
	case EEquipmentType::ET_Chest:
		switch (character)
		{
		case ECharacterType::CT_Uni:
			if (UUBArmor* armor = Cast<UUBArmor>(equipment)) {
				uniEquipment.Chest = armor;
			}
			break;
		case ECharacterType::CT_Tau:
			if (UUBArmor* armor = Cast<UUBArmor>(equipment)) {
				tauEquipment.Chest = armor;
			}
			break;
		case ECharacterType::CT_Elvasia:
			if (UUBArmor* armor = Cast<UUBArmor>(equipment)) {
				elvasiaEquipment.Chest = armor;
			}
			break;
		default:
			break;
		}
		break;
	case EEquipmentType::ET_Hand:
		switch (character)
		{
		case ECharacterType::CT_Uni:
			if (UUBArmor* armor = Cast<UUBArmor>(equipment)) {
				uniEquipment.Hand = armor;
			}
			break;
		case ECharacterType::CT_Tau:
			if (UUBArmor* armor = Cast<UUBArmor>(equipment)) {
				tauEquipment.Hand = armor;
			}
			break;
		case ECharacterType::CT_Elvasia:
			if (UUBArmor* armor = Cast<UUBArmor>(equipment)) {
				elvasiaEquipment.Hand = armor;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	
#pragma endregion
}

void UUBInventorySubsystem::UnEquip(EEquipmentType equipmenttype, ECharacterType character) {
	if (inventory.Num() >= INVENTORY_SIZE) {
		return;
	}
		
#pragma region swtichcase
	switch (equipmenttype)
	{
	case EEquipmentType::ET_Drone:
		switch (character)
		{
		case ECharacterType::CT_Uni:
			if (uniEquipment.Drone == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, uniEquipment.Drone->itemID);
			uniEquipment.Drone = nullptr;
			break;
		case ECharacterType::CT_Tau:
			if (tauEquipment.Drone == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, tauEquipment.Drone->itemID);
			tauEquipment.Drone = nullptr;
			break;
		case ECharacterType::CT_Elvasia:
			if (elvasiaEquipment.Drone == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, elvasiaEquipment.Drone->itemID);
			elvasiaEquipment.Drone = nullptr;
			break;
		default:
			break;
		}
		break;
	case EEquipmentType::ET_Weapon:
		switch (character)
		{
		case ECharacterType::CT_Uni:
			if (uniEquipment.Weapon == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, uniEquipment.Weapon->itemID);
			uniEquipment.Weapon = nullptr;
			break;
		case ECharacterType::CT_Tau:
			if (tauEquipment.Weapon == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, tauEquipment.Weapon->itemID);
			tauEquipment.Weapon = nullptr;
			break;
		case ECharacterType::CT_Elvasia:
			if (elvasiaEquipment.Weapon == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, elvasiaEquipment.Weapon->itemID);
			elvasiaEquipment.Weapon = nullptr;
			break;
		default:
			break;
		}
		break;
	case EEquipmentType::ET_Head:
		switch (character)
		{
		case ECharacterType::CT_Uni:
			if (uniEquipment.Head == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, uniEquipment.Head->itemID);
			uniEquipment.Head = nullptr;
			break;
		case ECharacterType::CT_Tau:
			if (tauEquipment.Head == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, tauEquipment.Head->itemID);
			tauEquipment.Head = nullptr;
			break;
		case ECharacterType::CT_Elvasia:
			if (elvasiaEquipment.Head == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, elvasiaEquipment.Head->itemID);
			elvasiaEquipment.Head = nullptr;
			break;
		default:
			break;
		}
		break;
	case EEquipmentType::ET_Chest:
		switch (character)
		{
		case ECharacterType::CT_Uni:
			if (uniEquipment.Chest == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, uniEquipment.Chest->itemID);
			uniEquipment.Chest = nullptr;
			break;
		case ECharacterType::CT_Tau:
			if (tauEquipment.Chest == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, tauEquipment.Chest->itemID);
			tauEquipment.Chest = nullptr;
			break;
		case ECharacterType::CT_Elvasia:
			if (elvasiaEquipment.Chest == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, elvasiaEquipment.Chest->itemID);
			elvasiaEquipment.Chest = nullptr;
			break;
		default:
			break;
		}
		break;
	case EEquipmentType::ET_Hand:
		switch (character)
		{
		case ECharacterType::CT_Uni:
			if (uniEquipment.Hand == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, uniEquipment.Hand->itemID);
			uniEquipment.Hand = nullptr;
			break;
		case ECharacterType::CT_Tau:
			if (tauEquipment.Hand == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, tauEquipment.Hand->itemID);
			tauEquipment.Hand = nullptr;
			break;
		case ECharacterType::CT_Elvasia:
			if (elvasiaEquipment.Hand == nullptr)
				return;
			AddItem(EItemType::IT_Equipment, elvasiaEquipment.Hand->itemID);
			elvasiaEquipment.Hand = nullptr;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
#pragma endregion
}

void UUBInventorySubsystem::SortInventory()
{
	if (inventory.Num() == 0)
		return;

	// 1. 키를 정렬
	TArray<int32> Keys;
	inventory.GetKeys(Keys);
	Keys.Sort();

	// 2. 새 Map 생성
	TMap<int32, TScriptInterface<IUBItem>> NewInventory;

	int32 NewIndex = 0;
	for (int32 OldKey : Keys)
	{
		if (!inventory.Contains(OldKey))
			continue;

		NewInventory.Add(NewIndex, inventory[OldKey]);
		NewIndex++;
	}

	// 3. 교체
	inventory = MoveTemp(NewInventory);
}

void UUBInventorySubsystem::SwapIndex(int index1, int index2)
{
	TScriptInterface<IUBItem> item1;
	TScriptInterface<IUBItem> item2;

	if (index1 == index2)
	{
		return;
	}
	if (inventory.Contains(index1))
	{
		item1 = inventory.Find(index1)->GetObject();
		inventory.Remove(index1);
	}
	if (inventory.Contains(index2))
	{
		item2 = inventory.Find(index2)->GetObject();
		inventory.Remove(index2);
	}

	if (item1 != nullptr) {
		inventory.Add(index2, item1);
	}
	if (item2 != nullptr) {
		inventory.Add(index1, item2);
	}
	SaveInventory();
	OnInventoryChanged.Broadcast();
}

TArray<FEquipmentStruct> UUBInventorySubsystem::GetEquipmentStructByGrade(EEquipmentGrade grade)
{
	TArray<FEquipmentStruct> equipments;

	UUBGameInstance* gameinstance = Cast< UUBGameInstance>(GetGameInstance());
	if (gameinstance == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("UUBGameInstance is nullptr"));
		return equipments;
	}

	if (UDataTable* datatable = gameinstance->EquipmentData)
	{
		for (const auto& RowPair : datatable->GetRowMap())
		{
			FEquipmentStruct* RowData = (FEquipmentStruct*)(RowPair.Value);

			if (!RowData)
			{
				continue;
			}

			// 조건 체크
			if (RowData->EquipmentGrade == grade)
			{
				equipments.Add(*RowData);
			}
		}
	}
	return equipments;
}

TArray<FEquipmentStruct> UUBInventorySubsystem::GetRandomEquipmentStruct(TArray<FEquipmentStruct> structs)
{
	TArray<FEquipmentStruct> equipments;
	if (structs.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("structs is empty"));
		return equipments;
	}
	
	int randomint = FMath::RandRange(1, 100);
	EEquipmentType randomtype;
	if (randomint <= 22) {
		randomtype = EEquipmentType::ET_Head;
	}
	else if (randomint <= 44) {
		randomtype = EEquipmentType::ET_Chest;
	}
	else if (randomint <= 66) {
		randomtype = EEquipmentType::ET_Drone;
	}
	else if (randomint <= 88) {
		randomtype = EEquipmentType::ET_Hand;
	}
	else{
		randomtype = EEquipmentType::ET_Weapon;
	}
	
	for (FEquipmentStruct equipment : structs) {
		if (equipment.EquipmentType == randomtype)
		{
			equipments.Add(equipment);
		}
	}
	return equipments;
}

FEquipmentStruct UUBInventorySubsystem::GetEquipmentInStruct(TArray<FEquipmentStruct> structs)
{
	if (structs.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("structs is empty"));
		return FEquipmentStruct();
	}
	int count = structs.Num();
	if (count == 0) {
		return FEquipmentStruct();
	}
	else if (count == 1) {
		return structs[0];
	}
	int randomint = FMath::RandRange(1, count);
	return structs[randomint - 1];
}

void UUBInventorySubsystem::Assemble(TArray<int32> indexlist)
{
	//아이템 등급 체크
	EEquipmentGrade itemgrade = IUBItem::Execute_GetItemGrade(inventory[indexlist[1]].GetObject());
	EEquipmentGrade targetgrade = EEquipmentGrade::EG_Common;
	//아이템들 삭제
	for (int32 index : indexlist) {
		RemoveItem(index);
	}
	if (itemgrade != EEquipmentGrade::EG_Legendary)
	{
		switch (itemgrade)
		{
		case EEquipmentGrade::EG_Common:
			targetgrade = EEquipmentGrade::EG_Rare;
			break;
		case EEquipmentGrade::EG_Rare:
			targetgrade = EEquipmentGrade::EG_Epic;
			break;
		case EEquipmentGrade::EG_Epic:
			targetgrade = EEquipmentGrade::EG_Legendary;
			break;
		}
	}
	else {
		return;
	}
	//아이템 생성
	TArray<FEquipmentStruct> equipments = GetEquipmentStructByGrade(targetgrade);
	TArray<FEquipmentStruct> randomequipments = GetRandomEquipmentStruct(equipments);
	FEquipmentStruct equipment = GetEquipmentInStruct(randomequipments);

	//아이템 Add
	AddItem(EItemType::IT_Equipment, equipment.ID);
}

void UUBInventorySubsystem::Disassemble(int32 index)
{
	//아이템 등급 체크
	EEquipmentGrade itemgrade = IUBItem::Execute_GetItemGrade(inventory[index].GetObject());
	//아이템 삭제
	RemoveItem(index);
	switch (itemgrade) {
	case EEquipmentGrade::EG_Legendary:
		AddItem(EItemType::IT_Material, 3,3);
		break;
	case EEquipmentGrade::EG_Epic:
		AddItem(EItemType::IT_Material, 2,3);
		break;
	case EEquipmentGrade::EG_Rare:
		AddItem(EItemType::IT_Material, 1,3);
		break;
	}
}

FUBStats UUBInventorySubsystem::GetEquippedStat(ECharacterType charactertype)
{
	FUBStats emptystat;
	FCharacterEquipmentStruct equipments;
	UUBGameInstance* gameinstance = Cast< UUBGameInstance>(GetGameInstance());
	if (gameinstance == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("UUBGameInstance is nullptr"));
		return emptystat;
	}

	emptystat = gameinstance->GetStatData(charactertype);

	switch (charactertype) {
		case ECharacterType::CT_Uni:
			equipments = uniEquipment;
			break;
		case ECharacterType::CT_Tau:
			equipments = tauEquipment;
			break;
		case ECharacterType::CT_Elvasia:
			equipments = elvasiaEquipment;
			break;
	}

	if (equipments.Weapon != nullptr) {
		emptystat.Attack += equipments.Weapon->statvalue;
	}
	if (equipments.Chest!= nullptr) {
		emptystat.MaxHP += equipments.Chest->statvalue;
	}
	if (equipments.Drone!= nullptr) {
		emptystat.parryPoint += equipments.Drone->statvalue;
	}
	if (equipments.Hand != nullptr) {
		emptystat.AttackSpeed += equipments.Hand->statvalue;
	}
	if (equipments.Head != nullptr) {
		emptystat.Defence += equipments.Head->statvalue;
	}
	return emptystat;
}
