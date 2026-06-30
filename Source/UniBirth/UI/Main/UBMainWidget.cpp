// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/UBMainWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UI/Main/UBTransparentUI.h"

void UUBMainWidget::ShowWidget()
{
	if (!TargetWidget)
		return;
	if (!TransparentWidget)
		return;

	CreatedTransparentWidget = Cast<UUBTransparentUI>(CreateWidget(this, TransparentWidget, FName("CreatedTransparentWidget")));
	if (!CreatedTransparentWidget)
		return;
	CreatedTransparentWidget->AddToViewport();
	CreatedTransparentWidget->OwnerWidget = this;



	CreatedWidget = CreateWidget(this, TargetWidget, FName("CreatedWidget"));
	if (!CreatedWidget)
		return;
	CreatedWidget->AddToViewport();

	auto widget = Cast< UUBMainWidget>(CreatedWidget);
	if (widget) {
		widget->OwnerWidget = this;
	}

}

void UUBMainWidget::HideWidget()
{
	if (!CreatedWidget)
		return;

	CreatedWidget->RemoveFromParent();
	CreatedWidget = nullptr;

	if (!CreatedTransparentWidget)
		return;

	CreatedTransparentWidget->RemoveFromParent();
	CreatedTransparentWidget = nullptr;
}

void UUBMainWidget::HoverWidget()
{

}

void UUBMainWidget::Tick(FGeometry MyGeometry, float InDeltaTime)
{

	FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	FVector2D pos = FVector2D(mousePos.X + 10, mousePos.Y + 10);
	if (!CreatedWidget)
		return;
	CreatedWidget->SetPositionInViewport(pos);

}
