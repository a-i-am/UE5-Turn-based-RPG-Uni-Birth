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


	//위젯 보이게하기
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
	//위젯 삭제
	CreatedWidget->RemoveFromParent();
	CreatedWidget = nullptr;

	if (!CreatedTransparentWidget)
		return;
	//위젯 삭제
	CreatedTransparentWidget->RemoveFromParent();
	CreatedTransparentWidget = nullptr;
}

void UUBMainWidget::HoverWidget()
{
	//위젯 생성 후 마우스에 따라다니게
}

void UUBMainWidget::Tick(FGeometry MyGeometry, float InDeltaTime)
{
	//만약 호버되어있으면 위젯 마우스 위치로 이동
	FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	FVector2D pos = FVector2D(mousePos.X + 10, mousePos.Y + 10);
	if (!CreatedWidget)
		return;
	CreatedWidget->SetPositionInViewport(pos);
	
}
