#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "UBDodgeKeySlot.generated.h"

UCLASS()
class UNIBIRTH_API UUBDodgeKeySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitSlot(TCHAR KeyChar);
	void SetMatched(bool bIsMatched);
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* keyImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UTexture2D*> keyIconsMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UniBirth")
	FLinearColor SuccessColor = FLinearColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UniBirth")
	FLinearColor FailColor = FLinearColor::Red;

private:
	FName assignedKeyName;
public:
	FName GetAssignedKeyName() const { return assignedKeyName; }
};
