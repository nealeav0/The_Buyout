// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityWidget.h"
#include "Ability.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "AbilityUpgradeWidget.h"

void UAbilityWidget::InitializeUI(FAbilityStruct Ability, int AIndex, UAbilityUpgradeWidget* HUDParent) {
	if (ActionLabel)
		ActionLabel->SetText(FText::FromString(Ability.AbilityName));
	
	if (LevelLabel)
		LevelLabel->SetText(FText::FromString(FString::Printf(TEXT("(Lv. %d)"), Ability.Level)));

	ActionButton->OnClicked.AddUniqueDynamic(this, &UAbilityWidget::OnAbilityClicked);

	/*if (ActionButton)
		ActionButton->SetIsEnabled(false);*/
	AbilityIndex = AIndex;
	ParentHUD = HUDParent;
}

void UAbilityWidget::EnableButton() {
	if (ActionButton)
		ActionButton->SetIsEnabled(true);
}

void UAbilityWidget::DisableButton() {
	if (ActionButton)
		ActionButton->SetIsEnabled(false);
}

void UAbilityWidget::OnAbilityClicked() {
	ParentHUD->OnAbilitySelect(AbilityIndex);
}