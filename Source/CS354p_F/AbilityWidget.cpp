// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityWidget.h"
#include "Ability.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UAbilityWidget::InitializeUI(FAbilityStruct Ability) {
	if (ActionLabel)
		ActionLabel->SetText(FText::FromString(Ability.AbilityName));
	
	if (LevelLabel)
		LevelLabel->SetText(FText::FromString(FString::Printf(TEXT("(Lv. %d)"), Ability.Level)));

	if (ActionButton)
		ActionButton->SetIsEnabled(false);
}

void UAbilityWidget::EnableButton() {
	if (ActionButton)
		ActionButton->SetIsEnabled(true);
}

void UAbilityWidget::DisableButton() {
	if (ActionButton)
		ActionButton->SetIsEnabled(false);
}