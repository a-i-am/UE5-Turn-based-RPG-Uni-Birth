// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "UIWiget.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUIWiget : public UUserWidget
{
	GENERATED_BODY()
  
    
public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> AtkBtn;

    UFUNCTION(BlueprintCallable)
    void AtkBtnClick();

  
};
