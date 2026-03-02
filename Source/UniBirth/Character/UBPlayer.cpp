// Fill out your copyright notice in the Description page of Project Settings.
#include "UBPlayer.h"
#include "Component/ActionSystemComponent.h"
#include "Battle/System/UBPlayerController.h"
#include  <Camera/CameraComponent.h>
#include "GameFramework/SpringArmComponent.h"
#include "Component/UBPlayerWidgetComponent.h"
#include "Component/UBCharacterUI.h"
#include "Component/UBStatsComponent.h"
#include "Component/UBComboBuffComponent.h"
#include "Component/UBBuffComponent.h"
#include "Common/SubSystem/UBInventorySubsystem.h"
AUBPlayer::AUBPlayer()
{
	teamType = ETeamType::Ally_Team;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprinArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0.0f, 10.f, 70.f));
	springArmComp->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	// °Å¸®
	springArmComp->TargetArmLength = 150.f;
	springArmComp->bUsePawnControlRotation = false;
	springArmComp->bDoCollisionTest = false;

	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	camComp->SetRelativeLocation(FVector(-265.0f, 191.0f, 18.0f));
	camComp->SetRelativeRotation(FQuat(FRotator(-0.3f, -2.0f, -4.f)));
	camComp->SetupAttachment(springArmComp);
	
	UIPosition = CreateDefaultSubobject<USceneComponent>(TEXT("UIPosition"));
	UIPosition->SetRelativeLocation(FVector(-140.0f, 14.0f, 19.0f));
	UIPosition->SetRelativeRotation(FRotator(5.f, -120.0f, 5.0f));
	UIPosition->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	UIPosition->SetupAttachment(RootComponent);
	
	comboBuffComp = CreateDefaultSubobject<UUBComboBuffComponent>(TEXT("ComboBuffComp"));

	GunMesh_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh_R"));
	GunMesh_R->SetupAttachment(GetMesh(), TEXT("Hand_Gun_R"));

	GunMesh_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh_L"));
	GunMesh_L->SetupAttachment(GetMesh(), TEXT("Hand_Gun_L"));



	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(RootComponent);
	DroneMesh->SetRelativeLocation(FVector(70.0f, 10.0f, 120.0f));
	DroneMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
}

bool AUBPlayer::operator<(const AUBPlayer& other) const
{
	return (statsComp->currentStats.AttackSpeed > other.statsComp->currentStats.Attack);
}

// Called when the game starts or when spawned
void AUBPlayer::BeginPlay()
{
	Super::BeginPlay();
	OriginalCamLocation = camComp->GetRelativeLocation();
	if (GunMesh_R && GunAsset_R)
	{
		GunMesh_R->SetSkeletalMesh(GunAsset_R);
	}
	if (GunMesh_L && GunAsset_L)
	{
		GunMesh_L->SetSkeletalMesh(GunAsset_L);
	}

	if (DroneMesh)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			UUBInventorySubsystem* InventorySubsystem = GI->GetSubsystem<UUBInventorySubsystem>();
			if (InventorySubsystem)
			{
				switch (characterType)
				{
				case ECharacterType::CT_Uni:
					if (InventorySubsystem->uniEquipment.Drone == nullptr) {
						DroneMesh->SetVisibility(false);
					}
					break;
				case ECharacterType::CT_Tau:
					if (InventorySubsystem->tauEquipment.Drone == nullptr) {
						DroneMesh->SetVisibility(false);
					}
					break;
				case ECharacterType::CT_Elvasia:
					if (InventorySubsystem->elvasiaEquipment.Drone == nullptr) {
						DroneMesh->SetVisibility(false);
					}
					break;
				}
				
			}
			
		}
	}
}

void AUBPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

}

void AUBPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUBPlayer::AttackHit()
{

}

void AUBPlayer::TOggleWeaponRotator()
{
	if (GunMesh_R)
	{
		bWeaponRotated = !bWeaponRotated;
		if (bWeaponRotated)
		{
			GunMesh_R->SetRelativeRotation(
				FRotator(
					-90.f, 
					0.f,   
					20.f   
				));
		}
		else
		{
			GunMesh_R->SetRelativeRotation(FRotator::ZeroRotator);
		}
	}

}

void AUBPlayer::DeathCharacter()
{
	Super::DeathCharacter();

	comboBuffComp->ResetBuffs(EComboType::GUARD, EDelCase::Death);
	comboBuffComp->ResetBuffs(EComboType::PARRY, EDelCase::Death);
	comboBuffComp->ResetBuffs(EComboType::DODGE, EDelCase::Death);
	BuffComp->DeleteBuffByReason(EDelCase::Death);
}


void AUBPlayer::SetStealthOpacity(float Alpha)
{
	if (USkeletalMeshComponent* getMesh = GetMesh())
	{
		getMesh->SetScalarParameterValueOnMaterials(TEXT("StealthAlpha"), Alpha);
		GunMesh_R->SetScalarParameterValueOnMaterials(TEXT("StealthAlpha"), Alpha);
	}
}


void AUBPlayer::EndStealth()
{
	statsComp->currentStats.CurrState.Remove(EUnitState::Stealth);
	statsComp->currentStats.critRate = 0.2f;
	SetStealthOpacity(1.0f);
}

void AUBPlayer::StartStealth()
{
	
	statsComp->currentStats.CurrState.Add(EUnitState::Stealth);
	statsComp->currentStats.critRate = 1.0f;
	SetStealthOpacity(0.35f);
}

