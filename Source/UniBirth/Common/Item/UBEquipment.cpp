// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/Item/UBEquipment.h"
#include "UBEquipment.h"


UTexture2D* UUBEquipment::GetIcon_Implementation() const
{
	if (icon.IsEmpty())
	{
		return nullptr;
	}

	// 아이콘들이 모여있는 공통 폴더
	const FString BasePath = TEXT("/Game/Assets/Texture/UI/Icon/");

	// Object Path 생성
	// /Game/UI/Icons/Sword_Icon.Sword_Icon
	const FString FullPath = BasePath + icon + TEXT(".") + icon;

	UTexture2D* Texture = LoadObject<UTexture2D>(
		nullptr,
		*FullPath
	);

	if (!Texture)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load icon: %s"), *FullPath);
	}

	return Texture;
}

EItemType UUBEquipment::GetItemType_Implementation() const
{
	return EItemType::IT_Equipment;
}

EEquipmentGrade UUBEquipment::GetItemGrade_Implementation() const
{
	return equipmentGrade;
}


void UUBEquipment::Equip()
{
}

void UUBEquipment::UnEquip()
{
}
