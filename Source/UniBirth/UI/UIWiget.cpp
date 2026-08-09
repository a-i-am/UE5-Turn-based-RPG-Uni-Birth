#include "UI/UIWiget.h"
#include "Components/Button.h"
#include "Character/UBPlayer.h"
#include "GameFramework/Character.h"
#include "Battle/System/UBPlayerController.h"
#include "Component/ActionSystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

void UUIWiget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AtkBtn)
	{
		AtkBtn->OnClicked.AddDynamic(this, &UUIWiget::OnAttackButtonClicked);
	}
}

void UUIWiget::OnAttackButtonClicked()
{
	AUBPlayerController* PC = Cast<AUBPlayerController>(GetOwningPlayer());
	if (!PC) return;
}

