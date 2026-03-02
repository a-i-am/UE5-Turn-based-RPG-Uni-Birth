// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/UBdefine.h"
#include <Common/Data/UBParryData.h>
#include "UI/Combat/UBBattleSelectBase.h"
#include "UBParryUI.generated.h"

UCLASS()
class UNIBIRTH_API UUBParryUI : public UUBBattleSelectBase
{
	GENERATED_BODY()
protected:

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual void NativeConstruct() override;// 위젯이 생성될 때 초기화하는 함수
public:
    // BindWidget 키워드를 통해, UI 블루프린트에 있는 위젯들과 C++ 코드를 연결시켜준다.
    // (중요) 이름이 반드시 UI 위젯의 이름과 똑같아야한다.
   
    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* parringOverlay;
     
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UImage* barAreaImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* parryPointImage;

    UPROPERTY(BlueprintReadWrite)
    float parryPointSpeed = 0.0f;

    UPROPERTY(meta = (BindWidget))
    class UImage* timingAreaImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* spaceBar;

    UPROPERTY(EditDefaultsOnly)
    class UDataTable* parryDataTable;

public:
    class UCanvasPanelSlot* parringOverlaySlotCache;
    float currParryPointWidth;
    float parryAreaWidth;
    TArray<float> timingAreas;
    bool bCheckParrying = false;
    float arrowPosX = 0.0f;

    // 초기화 (StartParry() 호출할 때마다 수행)
    UFUNCTION(BlueprintCallable)
    void StartParry(bool allDefence);
    
    UFUNCTION(BlueprintCallable)
    void SetComboBuffData(bool allDefence);

    UFUNCTION(BlueprintCallable)
    UUBStatsComponent* GetCurrPlayerStatComp();
    
    UFUNCTION(BlueprintCallable)
    void ApplyParryPointWidthData(float CurrPointWidth);

    // 패링 성공/실패 판정하기
    UFUNCTION(BlueprintCallable)
    void CheckParry();

    EParryzoneGrade ConvertTypeToParryGrade(ECharacterType CharType);

    void InitTimingAreas();
    template<typename T>
    void ShuffleTimingAreas(TArray<T>& TimingAreas)
    {
        // 배열이 비었거나 1개뿐이면 섞을 필요 없음
        if (TimingAreas.Num() <= 1) { return; }

        int32 LastIndex = TimingAreas.Num() - 1;

        for (int32 i = 0; i <= LastIndex; ++i)
        {
            int32 Index = FMath::RandRange(0, LastIndex);

            if (i != Index)
            {
                TimingAreas.Swap(i, Index);
            }
        }
    }
    void ApplyRandomizedPosition();
    void SetActionState();
    void MoveArrowTransform(float InDeltaTime);
    void OnTimeOut();

    FParryStruct* GetParryZoneData(EParryzoneGrade grade);
    //void GetSpeed();

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
    class UUBTimeProgressWidget* WBP_RectProgress;
    UPROPERTY()
    TObjectPtr<class ABattleManager> BM;
    
};
