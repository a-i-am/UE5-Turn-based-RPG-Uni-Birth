#include "UI/Combat/Combo/UBTimeProgressWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

void UUBTimeProgressWidget::NativeConstruct()
{
	Super::NativeConstruct();

	currentTime = defaultMaxTime;
}

void UUBTimeProgressWidget::SetMaxTime(float buffTime)
{
	bIsProgressRunning = true;
	maxTime = defaultMaxTime + buffTime;
	currentTime = maxTime;
}

void UUBTimeProgressWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsProgressRunning == false)
	{
		return;
	}
	currentTime -= InDeltaTime;

	if (currentTime <= 0.0f)
	{
		currentTime = 0;
		bIsProgressRunning = false;
		OnTimeEXpried.Broadcast();
	}

	if (timeProgressImg)
	{
		UMaterialInstanceDynamic* DynMat = timeProgressImg->GetDynamicMaterial();

		if (DynMat)
		{
			// 현재 비율 계산 (0.0 ~ 1.0)
			float PercentValue = currentTime / maxTime;

			DynMat->SetScalarParameterValue(FName("Percent"), PercentValue);

			GEngine->AddOnScreenDebugMessage(10, 2.0f, FColor::Green, FString::Printf(TEXT("Time left : %.2f sec"), PercentValue));
		}
	}


}