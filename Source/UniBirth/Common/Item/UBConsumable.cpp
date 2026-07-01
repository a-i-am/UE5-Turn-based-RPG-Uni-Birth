// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/Item/UBConsumable.h"
#include "UBConsumable.h"


UTexture2D* UUBConsumable::GetIcon_Implementation() const
{
	if (icon.IsEmpty())
	{
		return nullptr;
	}


	const FString BasePath = TEXT("/Game/Assets/Texture/UI/Icon/");



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

EItemType UUBConsumable::GetItemType_Implementation() const
{
	return EItemType::IT_Consumable;
}

EEquipmentGrade UUBConsumable::GetItemGrade_Implementation() const
{
	return EEquipmentGrade::EG_Common;
}
void UUBConsumable::Use()
{
}

