// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Combat/Combo/UBParryUI.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "kismet/GameplayStatics.h"
#include "Battle/System/UBPlayerController.h"
#include "Battle/SampleCharacter.h"
#include "Battle/BattleManager.h"
#include "Battle/System/BattleGameMode.h"
#include "UI/Combat/Combo/UBTimeProgressWidget.h"
#include "Component/UBStatsComponent.h"
#include "UI/Combat/UBCharacterPortraitList.h"
#include "UI/Combat/UBBattleHUD.h"

void UUBParryUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (parringOverlay)
	{
		parringOverlaySlotCache = Cast<UCanvasPanelSlot>(parringOverlay->Slot);
	}
	if (ABattleGameMode* GM = GetWorld()->GetAuthGameMode<ABattleGameMode>())
	{
		BM = GM->GetBattleManager();
	}
	if (BM == nullptr) return;

	UUBStatsComponent* StatComp = GetCurrPlayerStatComp();
	if (StatComp == nullptr)
	{
		return;
	}
	currParryPointWidth = StatComp->stats.parryPoint;
}

void UUBParryUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bCheckParrying)
	{
		MoveArrowTransform(InDeltaTime);
	}
	MoveArrowTransform(InDeltaTime);
}

void UUBParryUI::StartParry(bool AllDefence)
{
	if (AUBBattleHUD* HUD = Cast<AUBBattleHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		HUD->CurrentWidget = this;
	}
	BM->GlobalTimeStop();
	bCheckParrying = false;
	arrowPosX = 0;
	SetComboBuffData(AllDefence);
	SetActionState();
	InitTimingAreas();
	ShuffleTimingAreas(timingAreas);
	ApplyRandomizedPosition();

	SetVisibility(ESlateVisibility::Visible);
	WBP_RectProgress->SetMaxTime(0);
}

void UUBParryUI::SetComboBuffData(bool allDefence)
{
	if (allDefence)
	{
		for (auto iter : BM->GetAllPlayer())
		{
			UUBStatsComponent* StatComp = iter->statsComp.Get();

			if (StatComp->currentStats.parryPoint > 0)
			{
				currParryPointWidth = FMath::Max(currParryPointWidth, StatComp->currentStats.parryPoint);
			}
		}
	}
	else
	{
		UUBStatsComponent* StatComp = GetCurrPlayerStatComp();

		if (StatComp->currentStats.parryPoint > 0)
		{
			currParryPointWidth = StatComp->currentStats.parryPoint;
		}
	}

	ApplyParryPointWidthData(currParryPointWidth);
}

UUBStatsComponent* UUBParryUI::GetCurrPlayerStatComp()
{
	AUBCombatUnit* CurrMonster = BM->GetCurrentCharacter();

	if (IsValid(CurrMonster))
	{
		if (!(CurrMonster->CurrentTarget))
		{
			return nullptr;
		}
	}

	UUBStatsComponent* StatComp = CurrMonster->CurrentTarget->statsComp.Get();

	return StatComp;
}

void UUBParryUI::ApplyParryPointWidthData(float CurrPointWidth)
{
	UCanvasPanelSlot* ArrowCanvasSlot = Cast<UCanvasPanelSlot>(parryPointImage->Slot);

	if (ArrowCanvasSlot)
	{
		FVector2D CurrPointSize = ArrowCanvasSlot->GetSize();
		CurrPointSize.X = 10 * CurrPointWidth;
		ArrowCanvasSlot->SetSize(CurrPointSize);
	}
}


void UUBParryUI::InitTimingAreas()
{
 	if (!parringOverlaySlotCache || !timingAreaImage)
	{
		return;
	}

	AUBCombatUnit* CurrCharacter = BM->GetCurrentCharacter();

	if (!CurrCharacter)
	{
		return;
	}

	EParryzoneGrade TargetGrade = EParryzoneGrade::Base;
	ECharacterType CurrEnemyType = CurrCharacter->characterType;
	TargetGrade = ConvertTypeToParryGrade(CurrEnemyType);

	FParryStruct* ParryData = GetParryZoneData(TargetGrade);

	if (!ParryData)
	{
		return;
	}

	int32 SpawnedPosCount = 100;
	float OverlaySizeX = parringOverlaySlotCache->GetSize().X;
	float ZoneRatio = ParryData->ZoneWidth;

	parryAreaWidth = OverlaySizeX * (ZoneRatio / SpawnedPosCount);
	float SizeLimitRatio = 31.0f;

	UCanvasPanelSlot* TimingAreaCanvasSlot = Cast<UCanvasPanelSlot>(timingAreaImage->Slot);

	if (TimingAreaCanvasSlot)
	{
		TimingAreaCanvasSlot->SetSize(FVector2D(parryAreaWidth, parringOverlaySlotCache->GetSize().Y));
	}

	float SizeLimitWidth = OverlaySizeX * (SizeLimitRatio / SpawnedPosCount);

	if (parryAreaWidth >= SizeLimitWidth || parryAreaWidth > OverlaySizeX)
	{

		return;
	}


	int32 SegmentedCount = FMath::FloorToInt(OverlaySizeX / parryAreaWidth);

	timingAreas.Empty(SegmentedCount);

	for (int32 i = 0; i < SegmentedCount; ++i)
	{
		float XPos = i * parryAreaWidth;
		timingAreas.Add(XPos);
	}
}

void UUBParryUI::ApplyRandomizedPosition()
{

	if (timingAreas.Num() > 0)
	{

		int32 RandIndex = FMath::RandRange(0, timingAreas.Num() - 1);
		float PickedX = timingAreas[RandIndex];
		if (!parringOverlay && !parringOverlaySlotCache)
		{
			return;
		}

		UCanvasPanelSlot* CanvasSlot = Cast< UCanvasPanelSlot>(timingAreaImage->Slot);
		if (CanvasSlot)
		{
			CanvasSlot->SetPosition(FVector2D(PickedX, 0));
			UCanvasPanelSlot* SpaceBarCanvasSlot = Cast< UCanvasPanelSlot>(spaceBar->Slot);
			if (SpaceBarCanvasSlot)
			{
				SpaceBarCanvasSlot->SetPosition(FVector2D(PickedX + (CanvasSlot->GetSize().X * 0.5f), 0));
			}
		}
	}
}

void UUBParryUI::SetActionState()
{
	WBP_RectProgress->OnTimeEXpried.RemoveAll(this);
	BM->ForceSetActionState(EActionState::Parry);
	WBP_RectProgress->OnTimeEXpried.AddUObject(this, &UUBParryUI::OnTimeOut);
}

void UUBParryUI::OnTimeOut()
{
	BM->SetReactionResult(EResultType::Fail);
	BM->SetGlobalTimeReset();
	CloseWidget();

}

FParryStruct* UUBParryUI::GetParryZoneData(EParryzoneGrade grade)
{
	if (!parryDataTable)
	{
		return nullptr;
	}

	TArray<FParryStruct*> AllRows;
	parryDataTable->GetAllRows<FParryStruct>(TEXT(""), AllRows);

	for (FParryStruct* Row : AllRows)
	{
		if (Row && Row->ParryzoneGrade == grade)
		{
			return Row;
		}
	}
	return nullptr;
}

void UUBParryUI::MoveArrowTransform(float InDeltaTime)
{






	if (!parringOverlay || !parringOverlaySlotCache)
	{
		return;
	}
	if (!parryPointImage || bCheckParrying)
	{
		return;
	}

	UCanvasPanelSlot* ArrowCanvasSlot = Cast<UCanvasPanelSlot>(parryPointImage->Slot);
	float parryPointImageSizeX = ArrowCanvasSlot->GetSize().X;
	float OverlaySizeX = parringOverlaySlotCache->GetSize().X;
	float EndPosX = OverlaySizeX - parryPointImageSizeX;


	float Duration = 1.5f;
	if (parryPointSpeed < 0.0f)
	{
		parryPointSpeed = -(EndPosX / Duration);
	}
	else
	{
		parryPointSpeed = EndPosX / Duration;
	}

	arrowPosX += parryPointSpeed * InDeltaTime;
	ArrowCanvasSlot->SetPosition(FVector2D(arrowPosX, ArrowCanvasSlot->GetPosition().Y));



	if (arrowPosX >= EndPosX || arrowPosX <= 0.0f)
	{

		arrowPosX = FMath::Clamp(arrowPosX, 0.0f, EndPosX);
		parryPointSpeed *= -1.0f;
	}
}

void UUBParryUI::CheckParry()
{

	bCheckParrying = true;


	UCanvasPanelSlot* TimingCanvasSlot = Cast< UCanvasPanelSlot>(timingAreaImage->Slot);
	UCanvasPanelSlot* ArrowCanvasSlot = Cast< UCanvasPanelSlot>(parryPointImage->Slot);
	if (!TimingCanvasSlot || !ArrowCanvasSlot)
	{
		return;
	}

	float timingAreaPosX = TimingCanvasSlot->GetPosition().X;
	float timingAreaSizeX = TimingCanvasSlot->GetSize().X;

	float ALeft = ArrowCanvasSlot->GetPosition().X;
	float ARight = ArrowCanvasSlot->GetPosition().X + ArrowCanvasSlot->GetSize().X;
	float BLeft = timingAreaPosX;
	float BRight = timingAreaPosX + timingAreaSizeX;

	bool BOverlapX = (ALeft < BRight) && (ARight > BLeft);

	BM->SetReactionResult(BOverlapX ? EResultType::Success : EResultType::Fail);
	BM->SetGlobalTimeReset();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

EParryzoneGrade UUBParryUI::ConvertTypeToParryGrade(ECharacterType CharType)
{
	switch (CharType)
	{
	case ECharacterType::CT_Boss:
		return EParryzoneGrade::Boss;
	case ECharacterType::CT_Elite:
		return EParryzoneGrade::Elite;
	case ECharacterType::CT_Common:
	default:
		return EParryzoneGrade::Base;
	}
}


