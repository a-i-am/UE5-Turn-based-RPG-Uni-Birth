#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Common/UBdefine.h"
#include "UBParryData.generated.h"


USTRUCT(BlueprintType)
struct FParryStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EParryzoneGrade ParryzoneGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZoneWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Icon;
};

UCLASS()
class UNIBIRTH_API UBParryData : public UDataAsset
{
	GENERATED_BODY()
};
