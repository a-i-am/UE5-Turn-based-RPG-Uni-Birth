#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/UBdefine.h"
#include "UBComboBuffComponent.generated.h"

USTRUCT(BlueprintType)
struct FComboStatusInfo
{
	GENERATED_BODY()
	
	EBuffStatType statType;
	EStatOperator op;
	float value;
	int32 ID;
};
USTRUCT(BlueprintType)
struct FComboStatusList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FComboStatusInfo> statusCandidates;
};
USTRUCT(BlueprintType)
struct FComboLevelData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EComboLevel, FComboStatusList> levelMap;
};
USTRUCT(BlueprintType)
struct FActiveBuff
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EComboType type = EComboType::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EComboLevel level = EComboLevel::C;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EBuffStatType stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TableID;

	EStatOperator StatOp;

	float Value = 0.0f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNIBIRTH_API UUBComboBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUBComboBuffComponent();
	virtual void InitializeComponent();
	UFUNCTION(BlueprintCallable)
	void HandleBuffForceRemoved(int32 RemovedTableID);
protected:
	const int32 maxSlotCount = 3;
	TMap<EComboType ,TArray<FActiveBuff>> allActiveBuffs;
	TMap<EComboType, FComboLevelData> comboDataMap;
public:
	UFUNCTION(BlueprintCallable)
	void RequestAddComboBuff(EComboType InType);
	void ResetBuffs(EComboType type, EDelCase reason);

public:
	TMap<EComboType, TArray<FActiveBuff>> GetActiveBuffs()
	{
		return allActiveBuffs;
	}
private:
	FRandomStream ComboRandomStream;
private:
	void ApplyToBuffComponent(const FActiveBuff& InBuff);
	void RemoveActiveBuff(TArray<FActiveBuff>& BuffList, int32 Index);
	void ComboStatRateUpdate();
	int32 FindMergeTargetIndex(EComboType inType, EComboLevel inLevel);
	float GetComboTotalRate(EComboType ComboType);
	FActiveBuff GetFinalMergedBuff(EComboType Type, EComboLevel Level);
	FActiveBuff CreateActiveBuff(EComboType Type, EComboLevel Level);
	FComboStatusInfo* GetRandomStatus(EComboType inType, EComboLevel inLevel);
};
