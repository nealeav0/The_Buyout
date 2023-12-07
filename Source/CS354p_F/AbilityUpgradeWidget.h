// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityUpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UAbilityUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void InitializeUI(UAbilityManager* AbilityManagerIn);

	UFUNCTION()
	void SelectPlayer(float NavInput);

	UFUNCTION()
	void ConfirmPlayer();

	UFUNCTION()
	void ConfirmAbility();

	UFUNCTION()
	void CancelSelection();

	UFUNCTION()
	void SelectAbility(float NavInput);

	UFUNCTION()
	void OnWarriorClicked();
	
	UFUNCTION()
	void OnMageClicked();

	UFUNCTION()
	void OnRangerClicked();

	UFUNCTION()
	void OnUpgradeClicked();

	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnAbilitySelect(int AbilityIndex);

protected:
	UFUNCTION()
	void SetupAbilityUI(TArray<FAbilityStruct> PlayerAbilities);

	UFUNCTION()
	void ClearAbilityUI();


	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* WarriorButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* WarriorLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MageButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MageLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* RangerButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* RangerLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* AbilitiesContainer;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AbilityPointLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBackgroundBlur* ConfirmUpgradeMenu;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* UpgradeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ExitButton;

	UAbilityManager* AbilityManager;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAbilityWidget> AbilityWidgetClass;

	TArray<class UAbilityWidget*> AbilityWidgets;

	int PlayerIndex;

	int SelectedAbility;
};
