#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UBBattleHUD.generated.h"

class UUserWidget;
class AUBCharacterUI;
class UUBBattleSelectBase;

UCLASS()
class UNIBIRTH_API AUBBattleHUD : public AHUD
{
	GENERATED_BODY()

private:
	AUBBattleHUD();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Main")
	TSubclassOf<UUserWidget> MainUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Main")
	TObjectPtr<UUserWidget> MainUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Character")
	TSubclassOf<AUBCharacterUI> CharacterUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Character")
	TObjectPtr<AUBCharacterUI> CharacterUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Character")
	TObjectPtr<AUBCharacterUI> CharacterUIActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Monster")
	TSubclassOf<UUserWidget> MonsterHPClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Monster")
	TObjectPtr<UUserWidget> MonsterHPWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Select")
	TObjectPtr<UUBBattleSelectBase> CurrentWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	bool bIsAngry = false;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI|Effect")
	void ShowScreenGuardUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI|Effect")
	void HideScreenGuardUI();

	UFUNCTION()
	void OnTimeDelayEnded_Internal();

	UFUNCTION(BlueprintCallable, Category = "Battle")
	bool IsAngry() const { return bIsAngry; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void InitHUD();
};


