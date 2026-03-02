// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Data/CharacterPortraitData.h"

const FCharacterPortrait* UCharacterPortraitData::FindData(ECharacterType type)
{
    for (const FCharacterPortrait& Row : Portraits)
    {
        if (Row.CharacterType == type)
        {
            return &Row;
        }
    }
    return nullptr;

}