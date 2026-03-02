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
	// MO_AttackC -> Attack Type Notify
	AUBBaseMonster* CurrMonster = Cast<AUBBaseMonster>(BM->GetCurrentCharacter()); // 현재 몬스터
	if (CurrMonster == nullptr)
	{
		return;
	}

	skillData = CurrMonster->CurrentSkillData;

	if (nullptr == skillData)
	{
		return;
	}

	ENotifyType AtkType = skillData->FirstKey; // Left
	
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
	// 맵에 추가되어있는 모든 키를 AllowedKeys로 가져온다(Empty() 후에 가져옴)
	TArray<TCHAR> AllowedKeys;
	oppositeKeyMap.GetKeys(AllowedKeys);
	AllowedKeys.Remove(currentDisabledKey); // 금지된 키(선택된 키의 반대키)를 배열에서 제거한다 
	
	if (AllowedKeys.Num() < 0)
	{
		return TArray<TCHAR>(); // 빈 배열 반환
	}
	
	int32 ValidKeyCount = AllowedKeys.Num();
	
	// 예외처리 : 생성된 버튼 개수(키를 할당할 인덱스 i)가  WASD 개수보다 많으면 WASD 중에 랜덤 지정해서 버튼 추가
	if (visibleBtnCount > ValidKeyCount)
	{
		int32 NeededItemCount = visibleBtnCount - ValidKeyCount;

		AllowedKeys.Reserve(visibleBtnCount); // 필요한 크기의 메모리 미리 확보

		for (int i = 0; i < NeededItemCount; i++) // 뺀만큼 다시 채워주기
		{
			int32 RandomIndex = FMath::RandRange(0, ValidKeyCount - 1);

			// 기존에 있던 AllowedKeys의 참조를 그대로 Add하면 메모리주소 무효화->에러발생
			// 랜덤인덱스를 담을 값을 복사해서 추가한다
			TCHAR KeyToCopy = AllowedKeys[RandomIndex];
			AllowedKeys.Add(KeyToCopy);
		}
	}

	return AllowedKeys;
}

void UUBDodgeUI::SetActionState()
{
	WBP_RectProgress->OnTimeEXpried.RemoveAll(this);
	//player->SetActionState(
	// ::Dodge);
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
				ChildSlot->SetVisibility(ESlateVisibility::Hidden); // 일단 숨김
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

	// 전체 방어에 대한 처리
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

		// 몬스터가 타겟팅하고 있는 PC
		if (CurrMonster->CurrentTarget)
		{
			UUBStatsComponent* StatComp = CurrMonster->CurrentTarget->statsComp.Get();
			CurrentDodge = StatComp->currentStats.dodgeTime;
		}
	}

	AddDodgeMaxTime(CurrentDodge); // 초기 데이터 표시를 위해 한 번 실행
	
}

void UUBDodgeUI::AddDodgeMaxTime(int32 AddedTime)
{
	WBP_RectProgress->SetMaxTime(AddedTime);
}

void UUBDodgeUI::InitKeyCheckingContents(TArray<TCHAR> AllowedKeys)
{
	// 다시 회피해야하니까 초기화
	currentIndex = 0;

	Algo::RandomShuffle(AllowedKeys);

	// 슬롯 갱신
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
			// i=1 일때 AllowedKeys[0]을 가져와야 함
			else if (AllowedKeys.IsValidIndex(i - 1))
			{
				CharToSet = AllowedKeys[i - 1];
			}
			else
			{
				CharToSet = 'W'; // 초기화용 값
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
	 
	//if (bIsSuccess)
	//{
	//	BM->SetReactionResult(EResultType::Success);
	//}
	//else
	//{
	//	BM->SetReactionResult(EResultType::Fail);
	//}
}
 
void UUBDodgeUI::HidenDoggeCanvas()
{
	CloseWidget();
}
