// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/Item/UBMaterial.h"

UTexture2D* UUBMaterial::GetIcon_Implementation() const
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

EItemType UUBMaterial::GetItemType_Implementation() const
{
	return EItemType::IT_Material;
}

EEquipmentGrade UUBMaterial::GetItemGrade_Implementation() const
{
	return equipmentGrade;
}
