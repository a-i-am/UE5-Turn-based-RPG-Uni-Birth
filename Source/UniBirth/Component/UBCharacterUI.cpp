// UBCharacterUIComponent.cpp

#include "UBCharacterUI.h"
#include "Component/UBPlayerWidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Character/UBPlayer.h"
#include "Battle/SampleCharacter.h"
#include "UI/Combat/UBBattleHUD.h"
#include "Battle/System/BattleGameMode.h"
#include "UI/Combat/UBBattleSelectBase.h"
#include "Battle/BattleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Battle/System/UBPlayerController.h"
#include "Character/Data/UBCharacterSkillData.h"
#include "Battle/System/UBGameStateBase.h"
// Sets default values for this component's properties
AUBCharacterUI::AUBCharacterUI()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("UIRoot"));

	AttackWidget = CreateDefaultSubobject<UUBPlayerWidgetComponent>(TEXT("AttackWidget"));
	AttackWidget->uiType = EBattleUIType::BU_Attack;
	AttackWidget->SetupAttachment(RootComponent);

	ItemWidget = CreateDefaultSubobject<UUBPlayerWidgetComponent>(TEXT("ItemWidget"));
	ItemWidget->uiType = EBattleUIType::BU_Item;
	ItemWidget->SetupAttachment(RootComponent);

	SkillSelectWidget = CreateDefaultSubobject<UUBPlayerWidgetComponent>(TEXT("SkillSelectWidget"));
	SkillSelectWidget->uiType = EBattleUIType::BU_Skill;
	SkillSelectWidget->SetupAttachment(RootComponent);

	DefenceWidget = CreateDefaultSubobject<UUBPlayerWidgetComponent>(TEXT("DefenceWidget"));
	DefenceWidget->uiType = EBattleUIType::BU_Defence;
	DefenceWidget->SetupAttachment(RootComponent);

	TargetSelectWidget = CreateDefaultSubobject<UUBPlayerWidgetComponent>(TEXT("TargetSelectWidget"));
	TargetSelectWidget->uiType = EBattleUIType::BU_Attack;
	TargetSelectWidget->SetupAttachment(RootComponent);
	
}

void AUBCharacterUI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BM)
	{
		
	}

	
	if (OwnerPlayer) {
		UIPosition = OwnerPlayer->UIPosition;
	}
	if (UIPosition)
	{
		AttackWidget->SetWorldTransform(UIPosition->GetComponentToWorld());
		ItemWidget->SetWorldTransform(UIPosition->GetComponentTransform());
		SkillSelectWidget->SetWorldTransform(UIPosition->GetComponentTransform());
		DefenceWidget->SetWorldTransform(UIPosition->GetComponentTransform());
		TargetSelectWidget->SetWorldTransform(UIPosition->GetComponentTransform());
	}

}
 
void AUBCharacterUI::BeginPlay()
{
	Super::BeginPlay();

	TArray<UUBPlayerWidgetComponent*> OutArray;
	GetComponents(OutArray);
	ABattleGameMode* gm = GetWorld()->GetAuthGameMode<ABattleGameMode>();
	AUBGameStateBase* GS =GetWorld()->GetGameState<AUBGameStateBase>();
	GS->OnGameStart.AddUObject(this, &AUBCharacterUI::GameStartWiget);
	ABattleManager* bm = gm->GetBattleManager();
	if (bm == nullptr) return;
	//델리게이트 바인딩
	bm->OnMonsterAttackStarted.AddUObject(this, &AUBCharacterUI::HandleMonsterAttackStarted);
	bm->OnUIHide.AddUObject(this, &AUBCharacterUI::HandleHideWiget);
	bm->OnTimeDelayEnded.AddUObject(this, &AUBCharacterUI::HideAllWidget);

	for (UUBPlayerWidgetComponent* widget : OutArray) {
		switch (widget->uiType)
		{
		case EBattleUIType::BU_Attack:
			AttackWidget = widget;
			break;
		case EBattleUIType::BU_Skill:
			SkillSelectWidget = widget;
			break;
		case EBattleUIType::BU_Item:
			ItemWidget = widget;
			break;
		case EBattleUIType::BU_Defence:
			DefenceWidget = widget;
			break;
		case EBattleUIType::BU_SelectTarget:
			TargetSelectWidget = widget;
			break;
		}
		//@TODO
		if (!GetWorld()) return;

		
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			AUBBattleHUD* CachedBattleHUD = Cast<AUBBattleHUD>(PC->GetHUD());
		}
	}
	
	ChangeOwner(Cast<AUBPlayer>(GetWorld()->GetAuthGameMode<ABattleGameMode>()->GetBattleManager()->GetCurrentCharacter()));
	//ShowWidgetComp(AttackWidget);

	//AttackWidget = BattleHUD->AttackWidget;
	//ItemWidget = BattleHUD->ItemWidget;
	//SkillSelectWidget = BattleHUD->SkillSelectWidget;
	//DefenceWidget = BattleHUD->DefenceWidget;
	// 시작은 전부 숨김
	//HideAllWidget();
}

void AUBCharacterUI::SelectButton(EBattleUIType UIType)
{
	HideAllWidget();
	switch (UIType) {
	case EBattleUIType::BU_Attack:
		ShowWidgetComp(AttackWidget);
		break;
	case EBattleUIType::BU_Item:
		ShowWidgetComp(ItemWidget);
		break;
	case EBattleUIType::BU_Skill:
		ShowWidgetComp(SkillSelectWidget);
		break;
	case EBattleUIType::BU_Defence:
		break;
	case EBattleUIType::BU_SelectTarget:
		ShowWidgetComp(TargetSelectWidget);
		break;
	}
}

void AUBCharacterUI::GoBack()
{
	ShowWidgetComp(AttackWidget);
	HideWidgetComp(ItemWidget);
	HideWidgetComp(SkillSelectWidget);
	HideWidgetComp(DefenceWidget);
	HideWidgetComp(TargetSelectWidget);
}

void AUBCharacterUI::HideWidgetComp(UUBPlayerWidgetComponent* WidgetComp)
{
	if (!WidgetComp)
	{
		return;
	}
	// 1) 컴포넌트 레벨에서 숨기기 (렌더링 끄기)
	WidgetComp->SetVisibility(false);        // 언리얼 컴포넌트 visibility (true/false)
	WidgetComp->SetHiddenInGame(true);      // 게임 중 숨기기
	WidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 2) 내부 UUserWidget 레벨에서 숨기기 (슬레이트 레벨)
	if (UUserWidget* UW = WidgetComp->GetUserWidgetObject())
	{
		UW->SetVisibility(ESlateVisibility::Collapsed);
		if (UUBBattleSelectBase* uibase = Cast<UUBBattleSelectBase>(UW)) {
			uibase->isActiveWidget = false;
			if (AUBBattleHUD* HUD = Cast<AUBBattleHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
			{
				if (HUD->CurrentWidget == uibase)
				{
					HUD->CurrentWidget = nullptr;
				}
			}
			
		}
	}
}

void AUBCharacterUI::ShowWidgetComp(UUBPlayerWidgetComponent* WidgetComp)
{
	if (!WidgetComp) return;

	// 1) 컴포넌트 레벨에서 보이기
	WidgetComp->SetVisibility(true);
	WidgetComp->SetHiddenInGame(false);
	WidgetComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 2) 내부 UUserWidget 레벨에서 보이기
	if (UUserWidget* UW = WidgetComp->GetUserWidgetObject())
	{
		UW->SetVisibility(ESlateVisibility::Visible);
		if (UUBBattleSelectBase* uibase = Cast<UUBBattleSelectBase>(UW)) {
			uibase->isActiveWidget=true;
			if (AUBBattleHUD* HUD = Cast<AUBBattleHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
			{
				HUD->CurrentWidget = uibase;
			}

		}
	}
}

void AUBCharacterUI::HideAllWidget()
{
	HideWidgetComp(AttackWidget);
	HideWidgetComp(ItemWidget);
	HideWidgetComp(SkillSelectWidget);
	HideWidgetComp(DefenceWidget);
	HideWidgetComp(TargetSelectWidget);
	if (!GetWorld())
		return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
		return;

	AUBBattleHUD* HUD = Cast<AUBBattleHUD>(PC->GetHUD());
	if (!HUD)
		return;

	HUD->CurrentWidget = nullptr;
}

void AUBCharacterUI::ChangeOwner(AUBPlayer* NewOwner)
{
	OwnerPlayer = NewOwner;

	if (OwnerPlayer)
	{
		UIPosition = OwnerPlayer->UIPosition;		
		if (AUBPlayerController* PC = Cast<AUBPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			PC->OnPlayerTurnStarted(OwnerPlayer);
			PC->SetViewTargetWithBlend(OwnerPlayer, 0.2f);
		}

	}
	HideAllWidget();
}
void AUBCharacterUI::HandleMonsterAttackStarted(AUBCombatUnit* Target, FCharacterSkill* skill)
{
	//단체공격이라면.

	if (skill->target_scope == 3)
	{
		bFollowUIPosition = false;
	
		if (AUBBattleHUD* HUD =Cast<AUBBattleHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			HUD->ShowScreenGuardUI();

			//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.05f);
		}
		return;
	}

	if (OwnerPlayer != Target)
	{
		ChangeOwner(Cast<AUBPlayer>(Target));
		
	}

	HideAllWidget();

	ShowWidgetComp(DefenceWidget);
}

void AUBCharacterUI::HandleHideWiget()
{
	HideAllWidget();

}

void AUBCharacterUI::GameStartWiget()
{
	ShowWidgetComp(AttackWidget);
}

