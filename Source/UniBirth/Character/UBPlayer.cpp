#include "UBPlayer.h"
#include "Component/ActionSystemComponent.h"
#include "Battle/System/UBPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Component/UBPlayerWidgetComponent.h"
#include "Component/UBCharacterUI.h"
#include "Component/UBStatsComponent.h"
#include "Component/UBComboBuffComponent.h"
#include "Component/UBBuffComponent.h"
#include "Common/SubSystem/UBInventorySubsystem.h"

AUBPlayer::AUBPlayer()
{
	teamType = ETeamType::Ally;

	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 10.f, 70.f));
	SpringArmComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	SpringArmComponent->TargetArmLength = 150.f;
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CameraComponent->SetRelativeLocation(FVector(-265.0f, 191.0f, 18.0f));
	CameraComponent->SetRelativeRotation(FQuat(FRotator(-0.3f, -2.0f, -4.f)));
	CameraComponent->SetupAttachment(SpringArmComponent);

	UIPosition = CreateDefaultSubobject<USceneComponent>(TEXT("UIPosition"));
	UIPosition->SetRelativeLocation(FVector(-140.0f, 14.0f, 19.0f));
	UIPosition->SetRelativeRotation(FRotator(5.f, -120.0f, 5.0f));
	UIPosition->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	UIPosition->SetupAttachment(RootComponent);

	ComboBuffComponent = CreateDefaultSubobject<UUBComboBuffComponent>(TEXT("ComboBuffComp"));

	GunMesh_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh_R"));
	GunMesh_R->SetupAttachment(GetMesh(), TEXT("Hand_Gun_R"));

	GunMesh_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh_L"));
	GunMesh_L->SetupAttachment(GetMesh(), TEXT("Hand_Gun_L"));

	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(RootComponent);
	DroneMesh->SetRelativeLocation(FVector(70.0f, 10.0f, 120.0f));
	DroneMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
}

bool AUBPlayer::operator<(const AUBPlayer& Other) const
{
	return (statsComp->currentStats.AttackSpeed > Other.statsComp->currentStats.Attack);
}

void AUBPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (CameraComponent)
	{
		OriginalCamLocation = CameraComponent->GetRelativeLocation();
	}
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
				case ECharacterType::Uni:
					if (InventorySubsystem->uniEquipment.Drone == nullptr) {
						DroneMesh->SetVisibility(false);
					}
					break;
				case ECharacterType::Tau:
					if (InventorySubsystem->tauEquipment.Drone == nullptr) {
						DroneMesh->SetVisibility(false);
					}
					break;
				case ECharacterType::Elvasia:
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

void AUBPlayer::ToggleWeaponRotator()
{
	if (GunMesh_R)
	{
		bIsWeaponRotated = !bIsWeaponRotated;
		if (bIsWeaponRotated)
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

	if (ComboBuffComponent)
	{
		ComboBuffComponent->ResetBuffs(EComboType::Guard, EDelCase::Death);
		ComboBuffComponent->ResetBuffs(EComboType::Parry, EDelCase::Death);
		ComboBuffComponent->ResetBuffs(EComboType::Dodge, EDelCase::Death);
	}
	if (BuffComp)
	{
		BuffComp->DeleteBuffByReason(EDelCase::Death);
	}
}

void AUBPlayer::SetStealthOpacity(float InAlpha)
{
	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		CharacterMesh->SetScalarParameterValueOnMaterials(TEXT("StealthAlpha"), InAlpha);
		if (GunMesh_R)
		{
			GunMesh_R->SetScalarParameterValueOnMaterials(TEXT("StealthAlpha"), InAlpha);
		}
	}
}

void AUBPlayer::EndStealth()
{
	statsComp->currentStats.CurrState.Remove(EUnitState::Stealth);
	statsComp->currentStats.critRate = 0.2f;
	bIsStealth = false;
	SetStealthOpacity(1.0f);
}

void AUBPlayer::StartStealth()
{
	statsComp->currentStats.CurrState.Add(EUnitState::Stealth);
	statsComp->currentStats.critRate = 1.0f;
	bIsStealth = true;
	SetStealthOpacity(0.35f);
}


