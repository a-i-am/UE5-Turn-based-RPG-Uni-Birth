// Fill out your copyright notice in the Description page of Project Settings.

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
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UUBWeapon* Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUBArmor* Head;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUBArmor* Chest;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUBArmor* Hand;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUBArmor* Drone;
};


UCLASS()
class UNIBIRTH_API AUBPlayer : public AUBCombatUnit
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUBPlayer();
	bool operator<(const AUBPlayer& other) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class USpringArmComponent> springArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Camera")
	TObjectPtr<class UCameraComponent> camComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* UIPosition;

	virtual void AttackHit() override;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
	TObjectPtr<class UUBComboBuffComponent> comboBuffComp;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USkeletalMesh* GunAsset_R;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USkeletalMesh* GunAsset_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* GunMesh_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* GunMesh_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
	USkeletalMeshComponent* DroneMesh;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponHandType currentWeaponType;

	void TOggleWeaponRotator();
	bool bWeaponRotated = false;

	float AuccTime = 0.f;
	FVector BaseOffset;
	//UPROPERTY()
	//int32 CurrentSkillIndex = -1;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	//TObjectPtr<USceneComponent> CameraPivot;

	FVector OriginalCamLocation;
	virtual void DeathCharacter() override;

	void SetStealthOpacity(float Alpha);
	void EndStealth();
	UFUNCTION(BlueprintCallable)
	void StartStealth();

	bool isStealth = false;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ZoomInOut(bool isZoomin);
};
