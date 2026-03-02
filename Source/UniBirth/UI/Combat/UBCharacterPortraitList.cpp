// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Combat/UBCharacterPortraitList.h"
#include "Common/UBdefine.h"

void UUBCharacterPortraitList::ReApplyAll()
{
    for (auto portrait : portraitlist)
    {
        portrait->ReApplyPortrait();
    }
}

void UUBCharacterPortraitList::SetPortraitType(int index, ECharacterType type)
{
    if (index >= portraitlist.Num())
        return;
    portraitlist[index]->charactertype = type;
    portraitlist[index]->ReApplyPortrait();
}

void UUBCharacterPortraitList::NativeConstruct()
{
    Super::NativeConstruct();
    portraitlist.Add(CharacterPortrait_1);
    portraitlist.Add(CharacterPortrait_2);
    portraitlist.Add(CharacterPortrait_3);
    portraitlist.Add(CharacterPortrait_4);
    portraitlist.Add(CharacterPortrait_5);
    portraitlist.Add(CharacterPortrait_6);
    //portraitlist.Add(CharacterPortrait_7);
    //portraitlist.Add(CharacterPortrait_8);

    ReApplyAll();
}