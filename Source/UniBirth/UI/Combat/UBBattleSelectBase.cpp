// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Combat/UBBattleSelectBase.h"
#include "UI/Combat/UBBattleHUD.h"

void UUBBattleSelectBase::CloseWidget()
{

	if (AUBBattleHUD* HUD = Cast<AUBBattleHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{	
		this->SetVisibility(ESlateVisibility::Collapsed);
		if (HUD->CurrentWidget == this) {
			HUD->CurrentWidget = nullptr;
			
		}
		
	}
}
