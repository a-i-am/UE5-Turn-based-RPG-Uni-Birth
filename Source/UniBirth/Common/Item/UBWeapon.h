// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/Item/UBEquipment.h"
#include "Common/UBdefine.h"
#include "UBWeapon.generated.h"
/**
 * 
 */
UCLASS()
class UNIBIRTH_API UUBWeapon : public UUBEquipment
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	EBulletType bullettype;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	ECharacterType weapontype; //무기 주인장

};
