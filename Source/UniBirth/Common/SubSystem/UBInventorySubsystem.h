// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Common/Item/UBItem.h"
#include "Common/UBdefine.h"
#include "Character/UBPlayer.h"
#include "Character/Interface/FUBStats.h"
#include "UBInventorySubsystem.generated.h"

USTRUCT(Atomic,BlueprintType)
struct FInventoryStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	int32 slotindex;
	UPROPERTY(VisibleAnywhere)
	int32 itemID;
	UPROPERTY(VisibleAnywhere)
	int32 amount;
	UPROPERTY(VisibleAnywhere)
	EBulletType bullettype;
	UPROPERTY(VisibleAnywhere)
	EItemType itemtype;
};

USTRUCT(Atomic, BlueprintType)
struct FCharacterEquipmentSaveStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	int32 WeaponID = -1;
	UPROPERTY(VisibleAnywhere)
	int32 HeadID = -1;
	UPROPERTY(VisibleAnywhere)
	int32 ChestID = -1;
	UPROPERTY(VisibleAnywhere)
	int32 HandID = -1;
	UPROPERTY(VisibleAnywhere)
	int32 DroneID = -1;
};
/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnInventoryChanged);

UCLASS()
class UNIBIRTH_API UUBInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	TMap<int32,TScriptInterface<IUBItem>> inventory;
	UPROPERTY(BlueprintReadOnly)
	FCharacterEquipmentStruct uniEquipment;
	UPROPERTY(BlueprintReadOnly)
	FCharacterEquipmentStruct tauEquipment;
	UPROPERTY(BlueprintReadOnly)
	FCharacterEquipmentStruct elvasiaEquipment;

	UFUNCTION(BlueprintCallable)
	void SaveInventory();
	UFUNCTION(BlueprintCallable)
	void LoadInventory();
	UFUNCTION(BlueprintCallable)
	void SaveEquipment();
	UFUNCTION(BlueprintCallable)
	void LoadEquipment();
	UFUNCTION(BlueprintCallable)
	void Equip(UUBEquipment* equipment, ECharacterType character);
	UFUNCTION(BlueprintCallable)
	void UnEquip(EEquipmentType equipmenttype, ECharacterType character);


	UFUNCTION(BlueprintCallable)
	bool AddItem(EItemType itemtype, int32 itemID, int32 amount = 1, int32 invenindex = -1, EBulletType bullettype = EBulletType::BT_Normal);
	UFUNCTION(BlueprintCallable)
	bool RemoveItem(int32 slotindex);


	int32 GetConsumablesInInventory(int32 itemid);
	int32 GetMaterialsInInventory(int32 itemid);
	UFUNCTION(BlueprintCallable)
	class UUBConsumable* GetConsumablesByID(int32 itemid);

	UFUNCTION(BlueprintCallable)
	class UUBMaterial* GetMaterialsByID(int32 itemid);

	UFUNCTION(BlueprintCallable)
	class UUBEquipment* GetEquipmentsByID(int32 itemid);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FInventoryStruct> SavedInventory;
	UFUNCTION(BlueprintCallable)
	void SortInventory();
	UFUNCTION(BlueprintCallable)
	void SwapIndex(int index1, int index2);

	UFUNCTION(BlueprintCallable)
	TArray<FEquipmentStruct> GetEquipmentStructByGrade(EEquipmentGrade grade);
	UFUNCTION(BlueprintCallable)
	TArray<FEquipmentStruct> GetRandomEquipmentStruct(TArray<FEquipmentStruct> structs);
	UFUNCTION(BlueprintCallable)
	FEquipmentStruct GetEquipmentInStruct(TArray<FEquipmentStruct> structs);

	UFUNCTION(BlueprintCallable)
	void Assemble(TArray<int32> indexlist);
	UFUNCTION(BlueprintCallable)
	void Disassemble(int32 index);
	FOnInventoryChanged OnInventoryChanged;

	UFUNCTION(BlueprintCallable)
	FUBStats GetEquippedStat(ECharacterType charactertype);
};
