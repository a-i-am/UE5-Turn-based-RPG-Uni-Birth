#include "UI/Combat/Combo/UBDodgeUI.h"
#include "Battle/System/UBPlayerController.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Algo/RandomShuffle.h"
#include "TimerManager.h"
#include "Battle/BattleManager.h"
#include "Battle/System/BattleGameMode.h"
#include "Component/UBStatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Combat/Combo/UBTimeProgressWidget.h"
#include "Battle/SampleCharacter.h"
#include "Character/Enemy/UBBaseMonster.h"
#include "UI/Combat/UBBattleHUD.h"

const TCHAR KeyNames[] = { TEXT('A'), TEXT('D'), TEXT('W'), TEXT('S') };

void UUBDodgeUI::NativeConstruct()
{
	Super::NativeConstruct();
	if (ABattleGameMode* GM = GetWorld()->GetAuthGameMode<ABattleGameMode>())
	{
		BM = GM->GetBattleManager();
 		if (BM == nullptr) return;
	}

	oppositeKeyMap.Add(TEXT('W'), TEXT('S'));
	oppositeKeyMap.Add(TEXT('S'), TEXT('W'));
	oppositeKeyMap.Add(TEXT('A'), TEXT('D'));
	oppositeKeyMap.Add(TEXT('D'), TEXT('A'));
}

void UUBDodgeUI::InitSelectedKeyData()
{

	AUBBaseMonster* CurrMonster = Cast<AUBBaseMonster>(BM->GetCurrentCharacter());
	if (CurrMonster == nullptr)
	{
		return;
	}

	skillData = CurrMonster->CurrentSkillData;

	if (nullptr == skillData)
	{
		return;
	}

	ENotifyType AtkType = skillData->FirstKey;

	selectedFirstKey = ConvertTypeToFirstKey(AtkType);

	if (!CurrMonster || !skillData ||  !selectedFirstKey)
	{
		return;
	}

	int32 MaxKeyLength = skillData->MaxKeyLength;

	if (MaxKeyLength < minKeyLength)
	{
		return;
	}

	visibleBtnCount = FMath::RandRange(minKeyLength, MaxKeyLength);
}

TCHAR UUBDodgeUI::ConvertTypeToFirstKey(ENotifyType MonsterAtk)
{
	switch (MonsterAtk)
	{
	case ENotifyType::Left:
		return KeyNames[0];
	case ENotifyType::Right:
		return KeyNames[1];
	case ENotifyType::Up:
		return KeyNames[2];
	case ENotifyType::Down:
		return KeyNames[3];
	default: return 0;
	}
}

void UUBDodgeUI::InitOppositeKeyMap()
{
	if (oppositeKeyMap.Contains(selectedFirstKey))
	{
		currentDisabledKey = oppositeKeyMap[selectedFirstKey];
	}
}

TArray<TCHAR> UUBDodgeUI::InitKeyButtonsCount()
{

	TArray<TCHAR> AllowedKeys;
	oppositeKeyMap.GetKeys(AllowedKeys);
	AllowedKeys.Remove(currentDisabledKey);

	if (AllowedKeys.Num() < 0)
	{
		return TArray<TCHAR>();
	}

	int32 ValidKeyCount = AllowedKeys.Num();


	if (visibleBtnCount > ValidKeyCount)
	{
		int32 NeededItemCount = visibleBtnCount - ValidKeyCount;

		AllowedKeys.Reserve(visibleBtnCount);

		for (int i = 0; i < NeededItemCount; i++)
		{
			int32 RandomIndex = FMath::RandRange(0, ValidKeyCount - 1);



			TCHAR KeyToCopy = AllowedKeys[RandomIndex];
			AllowedKeys.Add(KeyToCopy);
		}
	}

	return AllowedKeys;
}

void UUBDodgeUI::SetActionState()
{
	WBP_RectProgress->OnTimeEXpried.RemoveAll(this);


	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("CALL SET "));
	BM->ForceSetActionState(EActionState::Dodge);

	WBP_RectProgress->OnTimeEXpried.AddUObject(this, &UUBDodgeUI::OnTimeOut);
}
void UUBDodgeUI::OnTimeOut()
{
	HidenDoggeCanvas();
	BM->SetReactionResult(EResultType::Fail);
	BM->SetGlobalTimeReset();
}

void UUBDodgeUI::StartDodge(bool allDefence)
{

	if (AUBBattleHUD* HUD = Cast<AUBBattleHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		HUD->CurrentWidget = this;
	}
	bResultIsDecided = false;
	BM->GlobalTimeStop();
	SetActionState();
	SetDodgeData(allDefence);
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (slotContainer)
	{
		keySlots.Empty();
		for (UWidget* Child : slotContainer->GetAllChildren())
		{
			if (UUBDodgeKeySlot* ChildSlot = Cast<UUBDodgeKeySlot>(Child))
			{
				keySlots.Add(ChildSlot);
				ChildSlot->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

	InitSelectedKeyData();
	InitOppositeKeyMap();
	if (keySlots.Num() > 0)
	{
		keySolved.Init(false, visibleBtnCount);
	}

	InitKeyCheckingContents(InitKeyButtonsCount());
}

void UUBDodgeUI::SetDodgeData(bool allDefence)
{
	float CurrentDodge = 0;


	if (allDefence)
	{
		for (auto iter : BM->GetAllPlayer())
		{
			UUBStatsComponent* StatComp = iter->statsComp.Get();
			CurrentDodge = FMath::Max(CurrentDodge, StatComp->currentStats.dodgeTime);
		}
	}
	else
	{
		AUBCombatUnit* CurrMonster = BM->GetCurrentCharacter();


		if (CurrMonster->CurrentTarget)
		{
			UUBStatsComponent* StatComp = CurrMonster->CurrentTarget->statsComp.Get();
			CurrentDodge = StatComp->currentStats.dodgeTime;
		}
	}

	AddDodgeMaxTime(CurrentDodge);

}

void UUBDodgeUI::AddDodgeMaxTime(int32 AddedTime)
{
	WBP_RectProgress->SetMaxTime(AddedTime);
}

void UUBDodgeUI::InitKeyCheckingContents(TArray<TCHAR> AllowedKeys)
{

	currentIndex = 0;

	Algo::RandomShuffle(AllowedKeys);


	for (int32 i = 0; i < visibleBtnCount; i++)
	{
		if (!keySlots.IsValidIndex(i))
		{
			break;
		}

		TCHAR CharToSet;

			if (i == 0)
			{
				CharToSet = selectedFirstKey;
			}

			else if (AllowedKeys.IsValidIndex(i - 1))
			{
				CharToSet = AllowedKeys[i - 1];
			}
			else
			{
				CharToSet = 'W';
			}

			keySlots[i]->InitSlot(CharToSet);
			keySlots[i]->SetVisibility(ESlateVisibility::Visible);
	}

	for (int32 i = visibleBtnCount; i < keySlots.Num(); i++)
	{
		keySlots[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUBDodgeUI::CheckDodgeKey(const FName pressedKey)
{
	if (bResultIsDecided)
	{
		return;
	}

	if (keySlots.Num() == 0)
	{
		return;
	}

	UUBDodgeKeySlot* CurrentSlot = keySlots[currentIndex];

	bool bIsMatch = (CurrentSlot->GetAssignedKeyName() == pressedKey);

	keySolved[currentIndex] = bIsMatch;
	CurrentSlot->SetMatched(bIsMatch);

	if (!bIsMatch)
	{
		bResultIsDecided = true;

		BM->SetReactionResult(EResultType::Fail);
		BM->SetGlobalTimeReset();

		FTimerDelegate TimerCallback;
		TimerCallback.BindUObject(this, &UUBDodgeUI::FinishDodgeGame, false);
		GetWorld()->GetTimerManager().SetTimer(EndGameTimerHandle, TimerCallback, resultDelay, false);

		WBP_RectProgress->bIsProgressRunning = false;

		return;
	}

	currentIndex++;

	if(currentIndex >= visibleBtnCount)
	{
		bResultIsDecided = true;
		WBP_RectProgress->bIsProgressRunning = false;

		BM->SetReactionResult(EResultType::Success);
		BM->SetGlobalTimeReset();

		FTimerDelegate TimerCallback;
		TimerCallback.BindUObject(this, &UUBDodgeUI::FinishDodgeGame, true);

		GetWorld()->GetTimerManager().SetTimer(EndGameTimerHandle, TimerCallback, resultDelay, false);
	}
}

void UUBDodgeUI::FinishDodgeGame(bool bIsSuccess)
{
	GetWorld()->GetTimerManager().ClearTimer(EndGameTimerHandle);

	HidenDoggeCanvas();









}

void UUBDodgeUI::HidenDoggeCanvas()
{
	CloseWidget();
}
