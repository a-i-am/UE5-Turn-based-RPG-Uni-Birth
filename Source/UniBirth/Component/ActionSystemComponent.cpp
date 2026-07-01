// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Animation/AnimMontage.h"
#include "Animation/UBAnimInstance.h"
#include "Character/UBPlayer.h"

UActionSystemComponent::UActionSystemComponent()
{


	PrimaryComponentTick.bCanEverTick = true;


}



void UActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();




}



void UActionSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}
UUBAnimInstance* UActionSystemComponent::GetUBAnimInstance()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return nullptr;

	USkeletalMeshComponent* Mesh = OwnerCharacter->FindComponentByClass<USkeletalMeshComponent>();
	if (!Mesh) return nullptr;

	return Cast<UUBAnimInstance>(Mesh->GetAnimInstance());

}


void UActionSystemComponent::OnActionSelected(FGameplayTag ActionTag)
{
	UUBAnimInstance* Anim = GetUBAnimInstance();
	if (Anim)
	Anim->PlayMontageGeneric(ActionTag);

}


