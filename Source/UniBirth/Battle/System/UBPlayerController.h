// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Common/FInputActionTagMapping.h"

#include "UBPlayerController.generated.h"




class AUBCombatUnit;
UCLASS()
class UNIBIRTH_API AUBPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:

	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;


	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputMappingContext> DefaultMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	TArray<TObjectPtr<class UUBInputAction>> InputMappings;

	UFUNCTION()
	void ForWardActionToASC(FGameplayTag tag);

	UFUNCTION(BlueprintCallable)
	void HandleInputAction(const FInputActionInstance& Instance);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_TargetLeft;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_TargetRight;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Confirm;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Return;

	virtual bool InputKey(const FInputKeyParams& Params) override;


		UPROPERTY()
		TArray<AUBCombatUnit*> TargetList;


		UPROPERTY()
		AUBCombatUnit* CurrentTarget = nullptr;


		int32 CurrentTargetIndex = 0;


		bool bIsTargetingMode = false;


		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		TObjectPtr<class UCameraComponent> TargetCamera;


		UPROPERTY()
		TObjectPtr<class ABattleManager> BM;


		void EnterPlayerTurnCamera();


		UFUNCTION(BlueprintCallable)
		void EnterTargetingMode();


		void OnPlayerTurnStarted(AUBCombatUnit * unit);


		void UpdateTargetCamera();


		void OnTargetLeft();


		void OnTargetRight();


		void OnConfirmAttack();


		void ChangeTarget(int32 Direction);

		void ShowCrosshair(bool show);

		UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
		TSubclassOf<UUserWidget> CrosshairWidgetClass;

		UPROPERTY()
		UUserWidget* CrosshairWidget;

		void FindTargetCamera();

		void CreateWiget();

		FTimerHandle TargetingDelayHandle;

		void FinishEnterTargetingMode();

		void ExitTargetingMode();

		void ForceEnterPlayerTurnCamera();


};
