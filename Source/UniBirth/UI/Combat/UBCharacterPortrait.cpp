// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Combat/UBCharacterPortrait.h"
#include "Components/Image.h"

void UUBCharacterPortrait::ReApplyPortrait()
{
    if (!Portrait)
    {
        UE_LOG(LogTemp, Warning, TEXT("ReApplyPortrait: Portrait is nullptr"));
        return;
    }
    if (!portraits)
    {
        UE_LOG(LogTemp, Warning, TEXT("ReApplyPortrait: portraits is nullptr"));
        return;
    }

    const FCharacterPortrait* Data = portraits->FindData(charactertype);
    if (!Data)
    {
        UE_LOG(LogTemp, Warning, TEXT("ReApplyPortrait: No data for type %d"), (int32)charactertype);
        return;
    }

    if (!Data->Texture)
    {
        UE_LOG(LogTemp, Warning, TEXT("ReApplyPortrait: Texture is nullptr for type %d"), (int32)charactertype);
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("ReApplyPortrait: Set texture %s for type %d"),
        *Data->Texture->GetName(), (int32)charactertype);

    Portrait->SetBrushFromTexture(Data->Texture);
}
