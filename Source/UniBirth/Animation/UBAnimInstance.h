// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Common/UBdefine.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "UBAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void PlayMontageGeneric(FGameplayTag ActionTag);
	virtual void NativeInitializeAnimation()override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<class UUBAnimationSet> animSet;

	UFUNCTION(BlueprintCallable)
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EActionState animCurrentState;

	UPROPERTY()
	TObjectPtr<class AUBBaseMonster> Monster;

	UPROPERTY()
	TObjectPtr<class ASampleCharacter> Player;

	UPROPERTY(BlueprintReadOnly)
	FTransform leftHandSocket;
};
