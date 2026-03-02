// Fill out your copyright notice in the Description page of Project Settings.



#include "UI/Main/UBCharacterSelectCheckBox.h"
#include "Kismet/GameplayStatics.h"
#include "Common/Data/UBSaveGame.h"
#include "Components/CheckBox.h"


void UUBCharacterSelectCheckBox::NativeConstruct()
{
    Super::NativeConstruct();

    if (Checkbox_Uni)
        Checkbox_Uni->OnCheckStateChanged.AddDynamic(this, &UUBCharacterSelectCheckBox::CheckCharacterList);

    if (Checkbox_Elvasia)
        Checkbox_Elvasia->OnCheckStateChanged.AddDynamic(this, &UUBCharacterSelectCheckBox::CheckCharacterList);

    if (Checkbox_Tau)
        Checkbox_Tau->OnCheckStateChanged.AddDynamic(this, &UUBCharacterSelectCheckBox::CheckCharacterList);

    
    InitCheckBox();
    
}


void UUBCharacterSelectCheckBox::CheckCharacterList(bool IsChecked)
{
    SelectedCharacters.Empty();

    if (Checkbox_Uni && Checkbox_Uni->IsChecked())
        SelectedCharacters.Add(ECharacterType::CT_Uni);

    if (Checkbox_Elvasia && Checkbox_Elvasia->IsChecked())
        SelectedCharacters.Add(ECharacterType::CT_Elvasia);

    if (Checkbox_Tau && Checkbox_Tau->IsChecked())
        SelectedCharacters.Add(ECharacterType::CT_Tau);

    SaveCharacterList();
}

void UUBCharacterSelectCheckBox::InitCheckBox()
{
    if (UUBSaveGame* LoadedGame = Cast<UUBSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainData"), 0)))
    {
        SelectedCharacters = LoadedGame->SelectedCharacters;
        if (SelectedCharacters.Contains(ECharacterType::CT_Tau)) {
            Checkbox_Tau->SetCheckedState(ECheckBoxState::Checked);
        }
        if (SelectedCharacters.Contains(ECharacterType::CT_Uni)) {
            Checkbox_Uni->SetCheckedState(ECheckBoxState::Checked);
        }
        if (SelectedCharacters.Contains(ECharacterType::CT_Elvasia)) {
            Checkbox_Elvasia->SetCheckedState(ECheckBoxState::Checked);
        }
    }
}

void UUBCharacterSelectCheckBox::SaveCharacterList()
{
    if (UUBSaveGame* SaveGameInstance = Cast<UUBSaveGame>(UGameplayStatics::CreateSaveGameObject(UUBSaveGame::StaticClass())))
    {
        SaveGameInstance->SelectedCharacters = SelectedCharacters;

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MainData"), 0);
    }

}
