#include "UI/Combat/Combo/UBDodgeKeySlot.h"
#include "Components/Image.h"

void UUBDodgeKeySlot::InitSlot(TCHAR KeyChar)
{
    FString KeyString = FString::Chr(KeyChar);
    assignedKeyName = FName(*KeyString);

    if (keyImage && keyIconsMap.Contains(KeyString))
    {
        UTexture2D* FoundTexture = keyIconsMap[KeyString];

        if (FoundTexture)
        {
            keyImage->SetBrushFromTexture(FoundTexture);
            keyImage->SetColorAndOpacity(FColor::White);
        }
    }

    SetVisibility(ESlateVisibility::Visible);
}

void UUBDodgeKeySlot::SetMatched(bool bIsMatched)
{
    if (!keyImage)
    {
        return;
    }

    if(bIsMatched)
    {
        keyImage->SetColorAndOpacity(SuccessColor);
    }
    else
    {

        keyImage->SetColorAndOpacity(FailColor);
    }
}
