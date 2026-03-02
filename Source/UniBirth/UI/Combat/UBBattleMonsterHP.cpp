#include "UI/Combat/UBBattleMonsterHP.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Component/UBComboBuffComponent.h"
#include "Component/UBStatsComponent.h"
#include "Component/HealthComponent.h"
#include "Battle/System/BattleGameMode.h"
#include "Battle/BattleManager.h"
#include "Common/System/UBGameInstance.h"
#include "Materials/MaterialInstanceDynamic.h"

void UUBBattleMonsterHP::NativeConstruct()
{
	Super::NativeConstruct();

	for (int i = 0; i < buffImageGrid->GetChildrenCount(); i++)
	{
		UImage* buffIcon = Cast<UImage>(buffImageGrid->GetChildAt(i));

		buffIcons.Add(buffIcon);
		buffIcon->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ABattleGameMode* GM = GetWorld()->GetAuthGameMode<ABattleGameMode>())
	{
		BM = GM->GetBattleManager();
		if (BM == nullptr)
		{
			return;
		}
	}
	
	UUBGameInstance* gameinstance = Cast< UUBGameInstance>(GetGameInstance());

	if (!gameinstance) 
	{
		return;
	}

	if (currCharacter)
	{
		ECharacterType CurrEnemyType = currCharacter->characterType;
		FUBStats EnemyStats = gameinstance->GetStatData(CurrEnemyType);
		MonsterName->SetText(FText::FromString(EnemyStats.CharacterDisplayName));
	}

	HP_Progress->SetPercent(1.0f);
	MP_Progress->SetPercent(1.0f);
	Shield_Progress->SetPercent(0.0f);

	if (currCharacter && currCharacter->BuffComp)
	{
		currCharacter->BuffComp->onBuffChangedDynamic.AddDynamic(this, &UUBBattleMonsterHP::UpdateBuffIcons);
	}
}

void UUBBattleMonsterHP::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!currCharacter)
	{
		return;
	}

	UHealthComponent* HealthComp = currCharacter->HealthComp;
	if (!IsValid(HealthComp))
	{
		return;
	}

	maxHP = currCharacter->statsComp->currentStats.MaxHP;
	currentHP = currCharacter->statsComp->currentStats.Hp;
	maxMP = currCharacter->statsComp->currentStats.MaxMp;
	currentMP = currCharacter->statsComp->currentStats.Mp;
	maxShield = 500000;
	currentShield = currCharacter->statsComp->currentStats.shield;

	UpdateHPBar(currentHP, maxHP);
	UpdateMPBar(currentMP, maxMP);
	UpdateShieldBar(currentShield, maxShield);
}

void UUBBattleMonsterHP::UpdateHPBar(float NewCurrentHP, float max)
{
	NewCurrentHP = FMath::Max(NewCurrentHP, 0.0f);

	if (IsValid(HP_Progress))
	{
		float barPercent = FMath::Clamp(NewCurrentHP / max, 0.0f, 1.0f);
		HP_Progress->SetPercent(barPercent);
	}
}

void UUBBattleMonsterHP::UpdateMPBar(float NewCurrentMP, float max)
{
	if (max <= 0.0f)
	{
		return;
	}

	NewCurrentMP = FMath::Max(NewCurrentMP, 0.0f);

	if (IsValid(MP_Progress))
	{
		float barPercent = FMath::Clamp(NewCurrentMP / max, 0.0f, 1.0f);
		MP_Progress->SetPercent(barPercent);
	}
}

void UUBBattleMonsterHP::UpdateShieldBar(float NewCurrentShield, float max)
{
	NewCurrentShield = FMath::Max(NewCurrentShield, 0.0f);

	if (IsValid(Shield_Progress))
	{
		float barPercent = FMath::Clamp(NewCurrentShield / max, 0.0f, 1.0f);
		Shield_Progress->SetPercent(barPercent);
	}
}

void UUBBattleMonsterHP::UpdateBuffIcons()
{
	if (!currCharacter || !currCharacter->BuffComp || !buffImageGrid)
	{
		return;
	}

	for (int32 i = 0; i < buffIcons.Num(); ++i)
	{
		if (buffIcons[i])
		{
			buffIcons[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	TArray<UTexture2D*> ActiveIcons = currCharacter->BuffComp->GetAllIcon();

	for (int32 i = 0; i < ActiveIcons.Num() && i < buffIcons.Num(); ++i)
	{
		if (buffIcons[i] && ActiveIcons[i])
		{
			buffIcons[i]->SetBrushFromTexture(ActiveIcons[i]);
			buffIcons[i]->SetVisibility(ESlateVisibility::Visible);
		}
	}
}