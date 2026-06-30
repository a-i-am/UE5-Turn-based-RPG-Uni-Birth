

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


	UFUNCTION(BlueprintCallable, Category = "UI")
	void GoBack();


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




	UFUNCTION(BlueprintCallable, Category = "UI")
	class AUBPlayer* GetOwnerPlayer() { return OwnerPlayer; }

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class AUBPlayer> OwnerPlayer;

	UPROPERTY(EditAnywhere)
	class AUBBattleHUD* BattleHUD;


	UPROPERTY(EditAnywhere)
	USceneComponent* UIPosition;




	UPROPERTY()
	TObjectPtr<class ABattleManager> BM;

	void HandleHideWiget();
	UPROPERTY()
	bool bFollowUIPosition = true;


	UFUNCTION()
	void GameStartWiget();
};
