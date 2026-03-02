// UBCharacterUIComponent.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Common/UBdefine.h"
#include "Character/Data/UBCharacterSkillData.h"
#include "UBCharacterUI.generated.h"


UCLASS()
class UNIBIRTH_API AUBCharacterUI : public AActor
{
	GENERATED_BODY()

public:
	AUBCharacterUI();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;


	// 디테일창에서 캐릭터에 붙어있는 WidgetComponent를 드래그해서 넣을 애들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class UUBPlayerWidgetComponent> AttackWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class UUBPlayerWidgetComponent> ItemWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class UUBPlayerWidgetComponent> SkillSelectWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class UUBPlayerWidgetComponent> DefenceWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class UUBPlayerWidgetComponent> TargetSelectWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class UUBPlayerWidgetComponent> ActvieSkillTargetSelectWidget;
	
	// 뒤로가기
	UFUNCTION(BlueprintCallable, Category = "UI")
	void GoBack();

	// 예시: 공격 버튼 눌렀을 때 같은 처리
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SelectButton(EBattleUIType UIType);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideWidgetComp(UUBPlayerWidgetComponent* WidgetComp);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowWidgetComp(UUBPlayerWidgetComponent* WidgetComp);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideAllWidget();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ChangeOwner(AUBPlayer* NewOwner);

	void HandleMonsterAttackStarted(class AUBCombatUnit* Target, FCharacterSkill* skill);

	/*UFUNCTION(BlueprintCallable, Category = "UI")
	void HideAllWorldUI();*/

	UFUNCTION(BlueprintCallable, Category = "UI")
	class AUBPlayer* GetOwnerPlayer() { return OwnerPlayer; }

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class AUBPlayer> OwnerPlayer;

	UPROPERTY(EditAnywhere)
	class AUBBattleHUD* BattleHUD;

	// UI가 따라갈 SceneComponent
	UPROPERTY(EditAnywhere)
	USceneComponent* UIPosition;

	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<UUserWidget> DefenceWidgetClass;

	UPROPERTY()
	TObjectPtr<class ABattleManager> BM;

	void HandleHideWiget();
	UPROPERTY()
	bool bFollowUIPosition = true;
	//void HandleMonsterAttackStarted(class AUBCombatUnit* Attacker, AUBCombatUnit* Target);

	UFUNCTION()
	void GameStartWiget();
};
