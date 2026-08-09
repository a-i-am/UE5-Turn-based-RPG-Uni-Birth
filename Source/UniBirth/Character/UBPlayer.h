#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GamePlayTags/UBGameplayTags.h"
#include "Common/UBdefine.h"
#include "Common/Item/UBArmor.h"
#include "Common/Item/UBWeapon.h"
#include "Common/Item/UBEquipment.h"
#include "Character/Interface/UBCombatUnit.h"
#include "Common/System/UBGameInstance.h"
#include "UBPlayer.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FCharacterEquipmentStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Equipment")
	TObjectPtr<UUBWeapon> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Equipment")
	TObjectPtr<UUBArmor> Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Equipment")
	TObjectPtr<UUBArmor> Chest;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Equipment")
	TObjectPtr<UUBArmor> Hand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Equipment")
	TObjectPtr<UUBArmor> Drone;
};

UCLASS()
class UNIBIRTH_API AUBPlayer : public AUBCombatUnit
{
	GENERATED_BODY()

public:
	AUBPlayer();
	bool operator<(const AUBPlayer& Other) const;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<USSceneComponent> UIPosition;

	virtual void AttackHit() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Buff")
	TObjectPtr<class UUBComboBuffComponent> ComboBuffComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<USkeletalMesh> GunAsset_R;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<USkeletalMesh> GunAsset_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> GunMesh_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> GunMesh_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
	TObjectPtr<USkeletalMeshComponent> DroneMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponHandType CurrentWeaponType;

	void ToggleWeaponRotator();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsWeaponRotated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float AccTime = 0.f;

	FVector BaseOffset;
	FVector OriginalCamLocation;

	virtual void DeathCharacter() override;

	void SetStealthOpacity(float InAlpha);
	void EndStealth();

	UFUNCTION(BlueprintCallable, Category = "State")
	void StartStealth();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsStealth = false;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Camera")
	void ZoomInOut(bool bInZoomIn);
};

