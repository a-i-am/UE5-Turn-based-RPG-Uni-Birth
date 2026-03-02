// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Common/FInputActionTagMapping.h"  

#include "UBPlayerController.generated.h"

/**
 * 
 */
class AUBCombatUnit;
UCLASS()
class UNIBIRTH_API AUBPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;

	//
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
	// ***************타겟 클로즈업 로직 **********************
		//타겟 리스트
		UPROPERTY()
		TArray<AUBCombatUnit*> TargetList;

		//현재 타겟
		UPROPERTY()
		AUBCombatUnit* CurrentTarget = nullptr;

		// 인덱스 조절
		int32 CurrentTargetIndex = 0;

		// 타겟팅모드일때만 키 입력받기 위해서
		bool bIsTargetingMode = false;

		//타겟 찍을 카메라
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		TObjectPtr<class UCameraComponent> TargetCamera;

		// BM 저장
		UPROPERTY()
		TObjectPtr<class ABattleManager> BM;

		// 플레이어 카메라, 턴 전환
		void EnterPlayerTurnCamera();

		// 타겟팅모드 시작
		UFUNCTION(BlueprintCallable)
		void EnterTargetingMode();

		// 플레이어 턴 시작
		void OnPlayerTurnStarted(AUBCombatUnit * unit);
	
		// 카메라 업데이트
		void UpdateTargetCamera();

		// 카메라 왼쪽이동
		void OnTargetLeft();

		// 카메라 오른쪽이동
		void OnTargetRight();

		// 공격 선택
		void OnConfirmAttack();

		// 타겟 선택 이동
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
     // *********************************************************
	
};
