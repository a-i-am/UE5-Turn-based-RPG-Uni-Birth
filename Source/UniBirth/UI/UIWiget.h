#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "UIWiget.generated.h"

class UButton;

UCLASS()
class UNIBIRTH_API UUIWiget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget), Category = "UI")
	TObjectPtr<UButton> AtkBtn;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnAttackButtonClicked();
};

