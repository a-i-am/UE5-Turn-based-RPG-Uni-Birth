// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Animation/AnimMontage.h"
#include "Animation/UBAnimInstance.h"
#include "Character/UBPlayer.h"
// Sets default values for this component's properties
UActionSystemComponent::UActionSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	//블랙보드 생성

	
}


// Called every frame
void UActionSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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


