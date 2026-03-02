#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameplayTagContainer.h"
#include "UBInputAction.generated.h"
UCLASS()
class UNIBIRTH_API UUBInputAction : public UInputAction
{
    GENERATED_BODY()

public:


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
    FGameplayTag ActionTag;

    /*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
    UAnimMontage* Montage;*/

  /*  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
    float Damage = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
    float Cooldown = 0.f;*/
};