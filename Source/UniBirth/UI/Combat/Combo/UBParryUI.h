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
    virtual void NativeConstruct() override;
public:



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


    UFUNCTION(BlueprintCallable)
    void StartParry(bool allDefence);

    UFUNCTION(BlueprintCallable)
    void SetComboBuffData(bool allDefence);

    UFUNCTION(BlueprintCallable)
    UUBStatsComponent* GetCurrPlayerStatComp();

    UFUNCTION(BlueprintCallable)
    void ApplyParryPointWidthData(float CurrPointWidth);


    UFUNCTION(BlueprintCallable)
    void CheckParry();

    EParryzoneGrade ConvertTypeToParryGrade(ECharacterType CharType);

    void InitTimingAreas();
    template<typename T>
    void ShuffleTimingAreas(TArray<T>& TimingAreas)
    {

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


    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
    class UUBTimeProgressWidget* WBP_RectProgress;
    UPROPERTY()
    TObjectPtr<class ABattleManager> BM;

};
