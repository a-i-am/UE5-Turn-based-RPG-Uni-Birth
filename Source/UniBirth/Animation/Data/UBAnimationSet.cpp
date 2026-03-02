// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Data/UBAnimationSet.h"

const FAnimActionData* UUBAnimationSet::FindData(FGameplayTag Tag)
{
    for (const FAnimActionData& Row : Actions)
    {
        if (Row.ActionTag == Tag)
            return   &Row;
    }
    return nullptr;
}
