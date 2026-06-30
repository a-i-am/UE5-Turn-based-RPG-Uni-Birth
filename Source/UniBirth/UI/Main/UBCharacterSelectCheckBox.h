// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/UBdefine.h"
#include "UBCharacterSelectCheckBox.generated.h"





UCLASS()
class UNIBIRTH_API UUBCharacterSelectCheckBox : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual void NativeConstruct() override;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* Checkbox_Uni;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* Checkbox_Tau;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* Checkbox_Elvasia;
	UPROPERTY(BlueprintReadWrite, Category="CharacterList",EditAnywhere)
	TArray<ECharacterType> SelectedCharacters;
	UFUNCTION()
	void CheckCharacterList(bool IsChecked);

	UFUNCTION()
	void InitCheckBox();

	UFUNCTION()
	void SaveCharacterList();
};
