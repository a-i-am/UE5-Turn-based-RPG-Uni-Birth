#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UBDodgeKeySlot.h"
#include "Character/Interface/UBCombatUnit.h"
#include "Character/Data/UBCharacterSkillData.h"
#include "UI/Combat/UBBattleSelectBase.h"
#include "UBDodgeUI.generated.h"

UCLASS()
class UNIBIRTH_API UUBDodgeUI : public UUBBattleSelectBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;// 위젯이 생성될 때 초기화하는 함수

public:
    UPROPERTY()
    TArray<bool> keySolved;

    UPROPERTY(meta = (BindWidget))
    class UPanelWidget* slotContainer;

    UPROPERTY(BlueprintReadWrite)
    int32 minKeyLength = 3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Delay", meta = (DisplayName = "결과 딜레이 시간"))
    float resultDelay = 0.02f;
public:
    
    bool bResultIsDecided = false;
    FTimerHandle EndGameTimerHandle;
    TMap<TCHAR, TCHAR> oppositeKeyMap;
    TArray<UUBDodgeKeySlot*> keySlots;
    int32 currentIndex = 0;
    int32 visibleBtnCount = 0;
    TCHAR selectedFirstKey;
    TCHAR currentDisabledKey;
    FCharacterSkill* skillData;

private:
    TCHAR ConvertTypeToFirstKey(ENotifyType MonsterAtk);
    TArray<TCHAR> InitKeyButtonsCount();
    void InitSelectedKeyData();
    void InitOppositeKeyMap();
    void InitKeyCheckingContents(TArray<TCHAR> AllowedKeys);

    UFUNCTION()
    void FinishDodgeGame(bool bIsSuccess);

public:
    UPROPERTY(meta = (BindWidget))
    class UUBTimeProgressWidget* WBP_RectProgress;

    UPROPERTY()
    TObjectPtr<class ABattleManager> BM;

    UFUNCTION(BlueprintCallable)
    void CheckDodgeKey(const FName pressedKey);
    UFUNCTION(BlueprintCallable)
    void SetDodgeData(bool allDefence);
    void AddDodgeMaxTime(int32 AddedTime);

    UFUNCTION(BlueprintCallable)
    void SetActionState();
    void HidenDoggeCanvas();
    void OnTimeOut();

    // 초기화 (StartParry() 호출할 때마다 수행)
    UFUNCTION(BlueprintCallable)
    void StartDodge(bool allDefence);
};
