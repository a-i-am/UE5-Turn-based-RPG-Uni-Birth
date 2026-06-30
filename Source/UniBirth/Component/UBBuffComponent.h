#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/UBdefine.h"
#include  "Character/Interface/FUBStats.h"
#include "UBBuffComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuffChangedDynamic);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuffForceRemoved, int32, FBuffSlot);



USTRUCT(BlueprintType)
struct FBuffSlot
{
	GENERATED_BODY()

	int32 ID;
	FString Icon;
	EBuffPriority priority;
	int32 CurrTurnCount = 0;
	int32 TurnDurationCount = 0;

	EBuffDurType DurType;

	TArray<EBuffStatType> BuffStats;
	EStatOperator StatOperator;
	TArray<float> Values = { 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUnitState State;
	TArray<EDelCase> DelCase;
	EBuffStackRule StackRule;
	int32 MaxStackCount;
	int32 CurrentStack = 0;
	TArray<float> BaseValues = { 0.0f };
	EBuffType BuffType;
	void MakeDelCase(FString DelCaseStr);


};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIBIRTH_API UUBBuffComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UUBBuffComponent();

	UPROPERTY(BlueprintAssignable)
	FOnBuffForceRemoved OnBuffForceRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnBuffChangedDynamic onBuffChangedDynamic;


protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	UUBBuffComponent* GetBuffComponent() const
	{
		return BuffComponent;
	}

	UPROPERTY()
	TObjectPtr<class ABattleManager> BM;
private:

	UUBBuffComponent* BuffComponent = nullptr;
	TArray<FBuffSlot> CurrBuffs[3];
	const int32 slotMax = 10;

	bool bCanUse = false;

public:
	UFUNCTION(BlueprintCallable)
	void AddBuffToStat(FBuffSlot newBuff, bool bApplyStat);

	UFUNCTION(BlueprintCallable)
	void DeleteBuff(FBuffSlot targetBuff);
	UFUNCTION(BlueprintCallable)
	void DeleteBuffByReason(EDelCase reason);
	void MakeSlot();
	int32 GetTotalNum();
	void RemoveTurnExpiredBuffs();

	UFUNCTION(BlueprintCallable)
	TArray<UTexture2D*> GetAllIcon() const;

	void ApplyStat(const FBuffSlot& newBuff, int32 Op);
	void AdvanceBuffTurns();
	void LogStatChange(const FString& CharName, const FBuffSlot& Buff, const FUBStats& Before, const FUBStats& After);
};
