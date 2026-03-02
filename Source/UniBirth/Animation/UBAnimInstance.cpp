// Fill out your copyright notice in the Description page of Project Settings.


#include "UBAnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "Data/UBAnimationSet.h"
#include  "Component/ActionSystemComponent.h"
#include"Battle/SampleCharacter.h"
#include "Battle/System/UBAIController.h"
#include "Character/Enemy/UBBaseMonster.h"
#include "Component/UBStatsComponent.h"
#include "GameFramework/Pawn.h"
void UUBAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    APawn* Pawn = TryGetPawnOwner();
    if (Pawn == nullptr) return;

    if (Monster = Cast<AUBBaseMonster>(Pawn))
    {
 
        animCurrentState = Monster->GetCurrentActionState();
    }
    else if (Player = Cast<ASampleCharacter>(Pawn))
    {
        animCurrentState = Player->GetCurrentActionState();
    }
}

void UUBAnimInstance::PlayMontageGeneric(FGameplayTag ActionTag)
{
    if (!animSet) return;
    const FAnimActionData* Row = animSet->FindData(ActionTag);
    if (!Row)
    {
        return;
    }
    if (!Row->Montage)
    {
        return;
    }
    Montage_Play(Row->Montage);
    //  델리게이트 생성
    FOnMontageEnded  EndDelegate;
    //이  델리게이트가 호출될때 반환할 함수 바인딩 (OnMontageEnd()) 
    EndDelegate.BindUObject(this, &UUBAnimInstance::OnMontageEnd);

    // RowMontage가  끝나면은  Endelegate를 호출하겠다.  선언
    Montage_SetEndDelegate(EndDelegate, Row->Montage);
}



void UUBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    
    Super::NativeUpdateAnimation(DeltaSeconds);
    if (Monster)
    {
        animCurrentState = Monster->GetCurrentActionState();
    }
    else if (Player)
    {
        animCurrentState = Player->GetCurrentActionState();
    }

    Player = Cast<ASampleCharacter>(TryGetPawnOwner());
    if (Player)
    {
        if (Player->currentWeaponType == EWeaponHandType::OneHandWeapon)
        {

            FTransform left_scoket = Player->GunMesh_R->GetSocketTransform(TEXT("Weapon_IK_Socket"), ERelativeTransformSpace::RTS_World);
            FVector outPostion;
            FRotator outRotator;
            Player->GetMesh()->TransformToBoneSpace(TEXT("Hand_R"), left_scoket.GetLocation(), FRotator::ZeroRotator,
                outPostion, outRotator);

            leftHandSocket.SetRotation(FQuat(outRotator));
            leftHandSocket.SetTranslation(outPostion);
        }
    }
}

void UUBAnimInstance::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
    Player = Cast<ASampleCharacter>(TryGetPawnOwner());
    if (Player)
    {
        Player->OnActionFinished();
    }
   

}

    


