// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define INVENTORY_SIZE 25

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	Ally_Team,
	Enemy_Team
};

UENUM(BlueprintType)
enum class EBattleUIType : uint8
{
	BU_Attack,
	BU_Skill,
	BU_Defence,
	BU_Item,
	BU_SelectTarget,
};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	CT_Uni,
	CT_Tau,
	CT_Elvasia,
	CT_Common,
	CT_Elite,
	CT_Boss,
};

UENUM(BlueprintType)
enum class ESkillKind : uint8
{
	SK_Passive UMETA(DisplayName = "패시브"),
	SK_Active UMETA(DisplayName = "액티브"),
	SK_Ultimate UMETA(DisplayName = "궁극기"),
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	None,
	MovingToTarget,
	Returning,
	Attack,
	ActiveSkill,
	UltiMateSkill,
	Die,
	idle,
	Hit,
	Parry,
	Dodge,
	Guard
};

UENUM(BlueprintType)
enum class EBulletType : uint8
{
	BT_Normal UMETA(DisplayName = "일반"),
	BT_Frost UMETA(DisplayName = "냉기"),
	BT_Flame UMETA(DisplayName = "화염"),
	BT_Laser UMETA(DisplayName = "레이저"),
};

UENUM(BlueprintType)
enum class EEquipmentGrade : uint8
{
	EG_Common UMETA(DisplayName = "일반"),
	EG_Rare UMETA(DisplayName = "희귀"),
	EG_Epic UMETA(DisplayName = "영웅"),
	EG_Legendary UMETA(DisplayName = "전설"),
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	ET_Weapon UMETA(DisplayName = "무기"),
	ET_Head UMETA(DisplayName = "머리"),
	ET_Chest UMETA(DisplayName = "몸통"),
	ET_Hand UMETA(DisplayName = "손"),
	ET_Drone UMETA(DisplayName = "드론"),
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_Equipment UMETA(DisplayName = "장비"),
	IT_Consumable UMETA(DisplayName = "소모품"),
	IT_Material UMETA(DisplayName = "장비조각"),

};
UENUM(BlueprintType)
enum class EResultType : uint8
{
	Success,
	Fail,
	None
};
UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	Range,
	Melee
};
UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	phase1,
	phase2,
	phase3
};

UENUM(BlueprintType)
enum class EParryzoneGrade : uint8
{
	Base  UMETA(DisplayName = "기본"),
	Elite UMETA(DisplayName = "정예"),
	Boss  UMETA(DisplayName = "보스")
};
UENUM(BlueprintType)
enum class ENotifyType : uint8
{
	Left,// A
	Right,  // D
	Up, // W
	Down, // S
	None,
};
UENUM(BlueprintType)
enum class PassiveType : uint8
{
	Uni_PASSIVE,
	Tau_PASSIVE,

};

UENUM(BlueprintType)
enum class EComboType : uint8
{
	GUARD,
	DODGE,
	PARRY,
	None
};

UENUM(BlueprintType)
enum class EComboLevel : uint8
{
	C,
	B,
	A,
	MAX
};

UENUM(BlueprintType)
enum class EDelCase : uint8
{
	ComboFailure,
	MyTurnEnd,
	RoundEnd,
	SkillHit,
	AttackHit,
	TakeDamage,
	Death,
	GameEnd,
	UseCriticalDamage
};

UENUM(BlueprintType)
enum class EBuffStatType : uint8
{
	Hp				UMETA(DisplayName = "체력"),
	MaxHp			UMETA(DisplayName = "최대 체력"),
	Mp				UMETA(DisplayName = "마나"),
	Attack			UMETA(DisplayName = "공격력"),
	CriticalRate	UMETA(DisplayName = "치명타 확률"),
	Defence			UMETA(DisplayName = "방어력"),
	Shield			UMETA(DisplayName = "실드"),
	Speed			UMETA(DisplayName = "공격 속도"),
	AtkDmgRate		UMETA(DisplayName = "공격 데미지 배율"),
	DefDmgRate		UMETA(DisplayName = "피격 데미지 배율"),
	StateResist		UMETA(DisplayName = "상태 이상 저항"),
	ParryPoint 		UMETA(DisplayName = "패링 포인트"),
	DodgeTime 		UMETA(DisplayName = "회피 입력시간"),
	State 			UMETA(DisplayName = "상태 이상"),
	AggroPoint 		UMETA(DisplayName = "어그로 포인트"),
};

UENUM(BlueprintType)
enum class EStatOperator : uint8
{
	Static			UMETA(DisplayName = "고정값"),
	Rate			UMETA(DisplayName = "비율값"),
};

UENUM(BlueprintType)
enum class EBuffPriority : uint8
{
	SurvivalCombo = 0 UMETA(DisplayName = "생존콤보"),
	Skill = 1			 UMETA(DisplayName = "스킬"),
	Item = 2			 UMETA(DisplayName = "아이템"),
};

UENUM(BlueprintType)
enum class EWeaponHandType : uint8
{
	OneHandWeapon,
	TwoHandWeapon
};

UENUM(BlueprintType)
enum class ECurrentAttackType : uint8
{
	Single,
	Multi
};
UENUM(BlueprintType)
enum class EUnitState : uint8
{
	None,
	Stealth,     // 은신
	Poisoning,	 // 중독
	Stun,        // 기절
	Burn,		 // 화상
};

UENUM(BlueprintType)
enum class EPassiveCondition : uint8
{
	Always,
	HpBelow50,
	HpBelow40,
	UsedSkillThisTurn,
	HasShield,
	StateEffect
};

UENUM(BlueprintType)
enum class EBuffDurType : uint8
{
	Permanent,      // 1) 영구
	TurnDuration,   // 2) 턴 지속
	StackPerTurn    // 3) 턴마다 강화
};

UENUM(BlueprintType)
enum class EBuffStackRule : uint8
{
	None,
	CalculateSum,
	ResetTurnCount
};

UENUM(BlueprintType)
enum class EBuffType : uint8
{
	Buff,
	DeBuff,
};