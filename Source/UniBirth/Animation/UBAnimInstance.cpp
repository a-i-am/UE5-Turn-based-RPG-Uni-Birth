#include "UBAnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "Data/UBAnimationSet.h"
#include "Component/ActionSystemComponent.h"
#include "Battle/SampleCharacter.h"
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
        AnimCurrentState = Monster->GetCurrentActionState();
    }
    else if (Player = Cast<ASampleCharacter>(Pawn))
    {
        AnimCurrentState = Player->GetCurrentActionState();
    }
}

void UUBAnimInstance::PlayMontageGeneric(FGameplayTag InActionTag)
{
    if (!AnimSet) return;
    const FAnimActionData* Row = AnimSet->FindData(InActionTag);
    if (!Row || !Row->Montage)
    {
        return;
    }
    Montage_Play(Row->Montage);

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &UUBAnimInstance::OnMontageEnd);

    Montage_SetEndDelegate(EndDelegate, Row->Montage);
}

void UUBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    if (Monster)
    {
        AnimCurrentState = Monster->GetCurrentActionState();
    }
    else if (Player)
    {
        AnimCurrentState = Player->GetCurrentActionState();
    }

    Player = Cast<ASampleCharacter>(TryGetPawnOwner());
    if (Player)
    {
        if (Player->currentWeaponType == EWeaponHandType::OneHandWeapon)
        {
            FTransform LeftSocketTransform = Player->GunMesh_R->GetSocketTransform(TEXT("Weapon_IK_Socket"), ERelativeTransformSpace::RTS_World);
            FVector OutPosition;
            FRotator OutRotator;
            Player->GetMesh()->TransformToBoneSpace(TEXT("Hand_R"), LeftSocketTransform.GetLocation(), FRotator::ZeroRotator,
                OutPosition, OutRotator);

            LeftHandSocket.SetRotation(FQuat(OutRotator));
            LeftHandSocket.SetTranslation(OutPosition);
        }
    }
}

void UUBAnimInstance::OnMontageEnd(UAnimMontage* InMontage, bool bInInterrupted)
{
    Player = Cast<ASampleCharacter>(TryGetPawnOwner());
    if (Player)
    {
        Player->OnActionFinished();
    }
}





