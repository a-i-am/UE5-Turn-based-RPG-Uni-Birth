#pragma once

#include "CoreMinimal.h"

constexpr int32 InventorySize = 25;

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	Ally,
	Enemy
};

UENUM(BlueprintType)
enum class EBattleUIType : uint8
{
	Attack,
	Skill,
	Defense,
	Item,
	SelectTarget
};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Uni,
	Tau,
	Elvasia,
	Common,
	Elite,
	Boss
};

UENUM(BlueprintType)
enum class ESkillKind : uint8
{
	Passive UMETA(DisplayName = "패시브"),
	Active UMETA(DisplayName = "액티브"),
	Ultimate UMETA(DisplayName = "궁극기")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	None,
	MovingToTarget,
	Returning,
	Attack,
	ActiveSkill,
	UltimateSkill,
	Die,
	Idle,
	Hit,
	Parry,
	Dodge,
	Guard
};

UENUM(BlueprintType)
enum class EBulletType : uint8
{
	Normal UMETA(DisplayName = "일반"),
	Frost UMETA(DisplayName = "냉기"),
	Flame UMETA(DisplayName = "화염"),
	Laser UMETA(DisplayName = "레이저")
};

UENUM(BlueprintType)
enum class EEquipmentGrade : uint8
{
	Common UMETA(DisplayName = "일반"),
	Rare UMETA(DisplayName = "희귀"),
	Epic UMETA(DisplayName = "영웅"),
	Legendary UMETA(DisplayName = "전설")
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Weapon UMETA(DisplayName = "무기"),
	Head UMETA(DisplayName = "머리"),
	Chest UMETA(DisplayName = "몸통"),
	Hand UMETA(DisplayName = "손"),
	Drone UMETA(DisplayName = "드론")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Equipment UMETA(DisplayName = "장비"),
	Consumable UMETA(DisplayName = "소모품"),
	Material UMETA(DisplayName = "장비조각")
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
	Phase1,
	Phase2,
	Phase3
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
	Left,
	Right,
	Up,
	Down,
	None
};

UENUM(BlueprintType)
enum class EPassiveType : uint8
{
	UniPassive,
	TauPassive
};

UENUM(BlueprintType)
enum class EComboType : uint8
{
	Guard,
	Dodge,
	Parry,
	None
};

UENUM(BlueprintType)
enum class EComboLevel : uint8
{
	C,
	B,
	A,
	Max
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
	HP				UMETA(DisplayName = "체력"),
	MaxHP			UMETA(DisplayName = "최대 체력"),
	MP				UMETA(DisplayName = "마나"),
	Attack			UMETA(DisplayName = "공격력"),
	CriticalRate	UMETA(DisplayName = "치명타 확률"),
	Defense			UMETA(DisplayName = "방어력"),
	Shield			UMETA(DisplayName = "실드"),
	Speed			UMETA(DisplayName = "공격 속도"),
	AtkDmgRate		UMETA(DisplayName = "공격 데미지 배율"),
	DefDmgRate		UMETA(DisplayName = "피격 데미지 배율"),
	StateResist		UMETA(DisplayName = "상태 이상 저항"),
	ParryPoint 		UMETA(DisplayName = "패링 포인트"),
	DodgeTime 		UMETA(DisplayName = "회피 입력시간"),
	State 			UMETA(DisplayName = "상태 이상"),
	AggroPoint 		UMETA(DisplayName = "어그로 포인트")
};

UENUM(BlueprintType)
enum class EStatOperator : uint8
{
	Static			UMETA(DisplayName = "고정값"),
	Rate			UMETA(DisplayName = "비율값")
};

UENUM(BlueprintType)
enum class EBuffPriority : uint8
{
	SurvivalCombo = 0 UMETA(DisplayName = "생존콤보"),
	Skill = 1			 UMETA(DisplayName = "스킬"),
	Item = 2			 UMETA(DisplayName = "아이템")
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
	Stealth,
	Poisoning,
	Stun,
	Burn
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
	Permanent,
	TurnDuration,
	StackPerTurn
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
	DeBuff
};

