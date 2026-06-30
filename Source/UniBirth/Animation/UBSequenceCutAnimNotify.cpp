// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UBSequenceCutAnimNotify.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "DefaultLevelSequenceInstanceData.h"
#include "Camera/CameraComponent.h"
#include "MovieSceneSequencePlayer.h"
#include "Delegates/Delegate.h"
#include "Character/Interface/UBCombatUnit.h"

void UUBSequenceCutAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

    if (AActor* Owner = MeshComp->GetOwner())
    {
        AUBCombatUnit* unit = Cast<AUBCombatUnit>(Owner);
        if(unit)
        unit->PlayCutscene(Sequence);
    }


}

