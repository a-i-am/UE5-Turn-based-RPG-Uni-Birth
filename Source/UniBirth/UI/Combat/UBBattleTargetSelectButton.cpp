// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Combat/UBBattleTargetSelectButton.h"
#include "Common/UBdefine.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Character/Interface/UBCombatUnit.h"

void UUBBattleTargetSelectButton::SetButtonImage()
{
    if (!TargetUnit)
        return;

    ECharacterType charactertype = TargetUnit->characterType;
    
    const FCharacterPortrait* Data = textures->FindData(charactertype);
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

	TargetImage->SetBrushFromTexture(Data->Texture);
}

void UUBBattleTargetSelectButton::Init(AUBCombatUnit* Unit)
{
    TargetUnit = Unit;
    SetButtonImage();
}
