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
        // 폰트->배경색 따로 글자색 따로 적용, 이미지->SetColorAndOpacity 하나면 된다)
        keyImage->SetColorAndOpacity(FailColor);
    }
}
