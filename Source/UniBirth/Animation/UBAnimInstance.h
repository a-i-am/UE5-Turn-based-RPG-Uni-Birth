#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Common/UBdefine.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "UBAnimInstance.generated.h"

class UUBAnimationSet;
class AUBBaseMonster;
class ASampleCharacter;

UCLASS()
class UNIBIRTH_API UUBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayMontageGeneric(FGameplayTag InActionTag);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void OnMontageEnd(UAnimMontage* InMontage, bool bInInterrupted);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Montage")
	TObjectPtr<UUBAnimationSet> AnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|State")
	EActionState AnimCurrentState;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Owner")
	TObjectPtr<AUBBaseMonster> Monster;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Owner")
	TObjectPtr<ASampleCharacter> Player;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|IK")
	FTransform LeftHandSocket;
};

