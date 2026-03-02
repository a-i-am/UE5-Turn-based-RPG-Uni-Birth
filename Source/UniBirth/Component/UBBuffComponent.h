#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/UBdefine.h"
#include  "Character/Interface/FUBStats.h"
#include "UBBuffComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuffChangedDynamic);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuffForceRemoved, int32, FBuffSlot);

// 10개 슬롯중에 한개에 대한 필요한 정보
// ID, Priority, 
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
	//ECharacterType characterType;
	UUBBuffComponent* BuffComponent = nullptr;
	TArray<FBuffSlot> CurrBuffs[3];	// 10개 슬롯 저장. 실제로 획득한 버프만 가지고 있을지.
	const int32 slotMax = 10;

	bool bCanUse = false;

public:
	UFUNCTION(BlueprintCallable)
	void AddBuffToStat(FBuffSlot newBuff, bool bApplyStat);

	UFUNCTION(BlueprintCallable)
	void DeleteBuff(FBuffSlot targetBuff); // 조건부로 특정 위치의 버프가 삭제될 때 
	UFUNCTION(BlueprintCallable)
	void DeleteBuffByReason(EDelCase reason);
	void MakeSlot(); // 슬롯 찼을 때 호출
	int32 GetTotalNum();
	void RemoveTurnExpiredBuffs();

	UFUNCTION(BlueprintCallable)
	TArray<UTexture2D*> GetAllIcon() const;

	void ApplyStat(const FBuffSlot& newBuff, int32 Op);
	void AdvanceBuffTurns();
	void LogStatChange(const FString& CharName, const FBuffSlot& Buff, const FUBStats& Before, const FUBStats& After);
};
