// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/System/UBPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "Battle/SampleCharacter.h"
#include "Character/UBPlayer.h"
#include "Containers/Array.h"
#include "InputAction.h"
#include "Common/FInputActionTagMapping.h"
#include "Component/ActionSystemComponent.h"
#include "Input/UBInputAction.h"
#include "UI/Combat/UBBattleHUD.h"
#include "Component/UBCharacterUI.h"
#include "UI/Combat/UBBattleSelectBase.h"
#include "Battle/BattleManager.h"
#include "Camera/CameraActor.h"
#include "Components/WidgetComponent.h"
#include "Battle/System/UBGameStateBase.h"
#include "Camera/CameraComponent.h"
#include "Battle/System/BattleGameMode.h"
void AUBPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (GetLocalPlayer() == nullptr)
        return;

    auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    Subsystem->AddMappingContext(DefaultMapping, 0);
    AUBGameStateBase* GS = GetWorld()->GetGameState<AUBGameStateBase>();
    //GS->OnGameStart.AddUObject(this, &AUBPlayerController::ForceEnterPlayerTurnCamera);

    CrosshairWidget = CreateWidget<UUserWidget>(this, CrosshairWidgetClass);
    if (CrosshairWidget)
    {
        CrosshairWidget->AddToViewport();
    }
    if (ABattleGameMode* GM = GetWorld()->GetAuthGameMode<ABattleGameMode>())
    {
        if (ABattleManager* BattleManager = GM->GetBattleManager())
        {
            BM = BattleManager;
        }
    }
    ShowCrosshair(false);

    OnPlayerTurnStarted(BM->currentCharacter);
}
void AUBPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
    {
        for (auto& Action : InputMappings)
        {
            if(Action)
            Input->BindAction(Action, ETriggerEvent::Triggered, this, &AUBPlayerController::HandleInputAction);

        }

        Input->BindAction(IA_TargetLeft,
            ETriggerEvent::Started,
            this,
            &AUBPlayerController::OnTargetLeft);

        Input->BindAction(IA_TargetRight,
            ETriggerEvent::Started,
            this,
            &AUBPlayerController::OnTargetRight);

        Input->BindAction(IA_Confirm,
            ETriggerEvent::Started,
            this,
            &AUBPlayerController::OnConfirmAttack);

        Input->BindAction(IA_Return,
            ETriggerEvent::Started,
            this,
            &AUBPlayerController::ExitTargetingMode);
    }

}

void AUBPlayerController::HandleInputAction(const FInputActionInstance& Instance)
{
    // 누른 키 가져오는 함수.
    const UUBInputAction* uAction = Cast<UUBInputAction>(Instance.GetSourceAction());
    if (uAction)
    {
        ForWardActionToASC(uAction->ActionTag);
    }
}
// 카메라 블렌드
void AUBPlayerController::EnterPlayerTurnCamera()
{
    FViewTargetTransitionParams Params;
    Params.BlendTime = 0.25f;
    Params.BlendFunction = EViewTargetBlendFunction::VTBlend_EaseInOut;
    Params.bLockOutgoing = true;

    if (GetPawn() == nullptr) {
        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT(" NULL PAWNs"));
    }

    GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT(" EnterPlayerTurnCamera"));
    if (GetViewTarget())
    {
        FString Msg = FString::Printf(
            TEXT("Before Cutscene ViewTarget: %s"),
            *GetViewTarget()->GetName()
        );

        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, Msg);
    }

    SetViewTargetWithBlend(GetPawn(), Params.BlendTime);
}
//타겟팅모드 
void AUBPlayerController::EnterTargetingMode()
{
    if (!BM) return;

    //타겟 가져오기
    TargetList = BM->targetList;
    if (TargetList.Num() == 0) {
        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT(" TargetList is null"));
        return;
    }
    CurrentTargetIndex = 0;
    CurrentTarget = TargetList[0];
 
    SetViewTargetWithBlend(BM, 0.25f);
    UpdateTargetCamera();
    ///여기에 딜레이 걸기

    GetWorld()->GetTimerManager().SetTimer(
        TargetingDelayHandle,
        this,
        &AUBPlayerController::FinishEnterTargetingMode,
        0.5f,   // 카메라 블렌드 시간과 맞추는 게 보통 좋음
        false
    );
    

}

void AUBPlayerController::OnPlayerTurnStarted(AUBCombatUnit* unit)
{
    if (unit == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT(" NULL UNIT"));
        return;
    }
    Possess(unit);
    
    FindTargetCamera();
    CreateWiget();
    EnterPlayerTurnCamera();
    //EnterTargetingMode();
}

void AUBPlayerController::UpdateTargetCamera()
{
    if (!CurrentTarget || !TargetCamera)
    {
        return;
    }
    FVector Focus =
        CurrentTarget->GetActorLocation() + FVector(0, -450, 70.f);

    FRotator CamRot = CurrentTarget->GetActorRotation();
    CamRot.Pitch = 0.f; 
    CamRot.Roll = 0.f;
    CamRot.Yaw = 90.0f;

    BM->SetActorLocation(Focus);
    BM->SetActorRotation(CamRot);

}

void AUBPlayerController::OnTargetLeft()
{
    if (bIsTargetingMode == false) return;
    ChangeTarget(-1);
}

void AUBPlayerController::OnTargetRight()
{
    if (bIsTargetingMode == false) return;
    ChangeTarget(+1);
}



void AUBPlayerController::ForWardActionToASC(FGameplayTag tag)
{
    APawn* pawn = GetPawn();
    if (!pawn)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, TEXT("null pawn"));
    }
    UActionSystemComponent* asc = pawn->FindComponentByClass<UActionSystemComponent>();
    if (!asc) return;
    asc->OnActionSelected(tag);
}

void AUBPlayerController::OnConfirmAttack()
{
    if (!bIsTargetingMode || !CurrentTarget) return;

    bIsTargetingMode = false;
    ShowCrosshair(false);
    BM->TargetAttack(CurrentTarget);
}

void AUBPlayerController::ChangeTarget(int32 Direction)
{
    if (!bIsTargetingMode || TargetList.Num() == 0) return;

    CurrentTargetIndex = (CurrentTargetIndex + Direction + TargetList.Num()) % TargetList.Num();
    CurrentTarget = TargetList[CurrentTargetIndex];
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
    UpdateTargetCamera();
}

void AUBPlayerController::ShowCrosshair(bool show)
{
    if (CrosshairWidget == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("NULL WIGET"));
        return;
    }
    CrosshairWidget->SetVisibility(
        show ? ESlateVisibility::Visible
        : ESlateVisibility::Hidden
    );
}

void AUBPlayerController::FindTargetCamera()
{
    if (!BM) return;
    if (BM->TargetCamera == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("NULL CAMERA"));
        return;
    }
      TargetCamera = BM->TargetCamera;
      GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT(" FindTargetCamera"));
}




void AUBPlayerController::CreateWiget()
{
   if (CrosshairWidgetClass)
   {
       CrosshairWidget =
           CreateWidget<UUserWidget>(this, CrosshairWidgetClass);

       if (CrosshairWidget)
       {
           CrosshairWidget->AddToViewport();
           ShowCrosshair(false);
       }
   }
}

bool AUBPlayerController::InputKey(const FInputKeyParams& Params)
{
    bool bresult = Super::InputKey(Params);
 
    AUBBattleHUD* hud = GetHUD<AUBBattleHUD>();
    if (hud == nullptr) {
        return bresult;
    }
    if (Params.Event != EInputEvent::IE_Pressed) {
        return bresult;
    }
    //@TODO
    if (bIsTargetingMode == true) return bresult;
    UUBBattleSelectBase* currentwidget= nullptr;
    if (hud->CharacterUI != nullptr) {
        currentwidget = hud->CurrentWidget;
    }
    if (currentwidget == nullptr) {
        return bresult;
    }
    if (Params.Key == EKeys::Q) {

        currentwidget->PressQ();
    }
    else if (Params.Key == EKeys::W) {
        currentwidget->PressW();
    }
    else if (Params.Key == EKeys::E) {
        currentwidget->PressE();
    }
    else if (Params.Key == EKeys::A) {
        currentwidget->PressA();
    }
    else if (Params.Key == EKeys::S) {
        currentwidget->PressS();
    }
    else if (Params.Key == EKeys::D) {
        currentwidget->PressD();
    }
    else if (Params.Key == EKeys::SpaceBar) {
        currentwidget->PressSpace();
    }
    GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan,Params.Key.ToString());
    return bresult;
}

void AUBPlayerController::FinishEnterTargetingMode()
{
    bIsTargetingMode = true;
    ShowCrosshair(true);
}

void AUBPlayerController::ExitTargetingMode()
{
    if (bIsTargetingMode == false)
        return;
    bIsTargetingMode = false;
    ShowCrosshair(false);
    EnterPlayerTurnCamera();
   
}

void AUBPlayerController::ForceEnterPlayerTurnCamera()
{
    FViewTargetTransitionParams Params;
    Params.BlendTime = 0.5f;
    Params.BlendFunction = EViewTargetBlendFunction::VTBlend_EaseInOut;
    Params.bLockOutgoing = true;

    GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("ForceEnterPlayerTurnCamera"));
    SetViewTargetWithBlend(GetPawn(), Params.BlendTime);

    if (GetViewTarget())
    {
        FString Msg = FString::Printf(
            TEXT("after Cutscene ViewTarget: %s"),
            *GetViewTarget()->GetName()
        );

        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, Msg);
    }
}