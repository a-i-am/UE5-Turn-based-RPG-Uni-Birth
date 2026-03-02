#pragma once
#include "NativeGameplayTags.h"

namespace UBGameplayTags
{
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Attack_Normal);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Attack_Skill);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Attack_Ultimate);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Counter);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Parry);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Dodge);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Hit);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Down);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Death);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_StandUp);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Defence);

    // 몬스터 전용 행동
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Monster_AttackA);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Monster_AttackB);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Monster_AttackC);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Monster_AttackD);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Monster_Hit);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Monster_Down);

}
