// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityUpgradeWidget.h"
#include "EntityBase.h"
#include "BattleHUD.h"
#include "AbilityWidget.h"
#include "AbilityManager.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"

void UAbilityUpgradeWidget::InitializeUI(UAbilityManager* AbilityManagerIn) {

    AbilityManager = AbilityManagerIn;

    TArray<FEntityStruct> PlayerStructs = AbilityManager->GetPlayersArray();
    if (WarriorLabel)
        WarriorLabel->SetText(FText::FromString(PlayerStructs[0].EntityName));

    if (MageLabel)
        MageLabel->SetText(FText::FromString(PlayerStructs[1].EntityName));

    if (RangerLabel)
        RangerLabel->SetText(FText::FromString(PlayerStructs[2].EntityName));

    if (WarriorButton)
        WarriorButton->SetIsEnabled(true);

    if (MageButton)
        MageButton->SetIsEnabled(false);

    if (RangerButton)
        RangerButton->SetIsEnabled(false);

    AbilityManager->PlayerIndex = 0;
}

void UAbilityUpgradeWidget::SelectPlayer(float NavInput) {

    // Unselect PlayerIndex on UI
    switch (AbilityManager->PlayerIndex)
    {
    case 0:
        WarriorButton->SetIsEnabled(false);
        break;
    case 1:
        MageButton->SetIsEnabled(false);
        break;
    case 2:
        RangerButton->SetIsEnabled(false);
        break;
    default:
        break;
    }

    AbilityManager->SelectPlayer(NavInput);

    // Select PlayerIndex on UI
    switch (AbilityManager->PlayerIndex)
    {
    case 0:
        WarriorButton->SetIsEnabled(true);
        break;
    case 1:
        MageButton->SetIsEnabled(true);
        break;
    case 2:
        RangerButton->SetIsEnabled(true);
        break;
    default:
        break;
    }
}

void UAbilityUpgradeWidget::SelectAbility(float NavInput) {

    // Unselect Ability on UI
    AbilityWidgets[AbilityManager->AbilityIndex]->DisableButton();

    AbilityManager->SelectAbility(NavInput);

    AbilityWidgets[AbilityManager->AbilityIndex]->EnableButton();
    // Select Ability on UI

}


void UAbilityUpgradeWidget::ConfirmPlayer() {
    AbilityManager->ConfirmSelection();
     
    // Setup ability UI
    TArray<FAbilityStruct> PlayerAbilities;
    switch (AbilityManager->PlayerIndex)
    {
    case 0:
        PlayerAbilities = AbilityManager->WarriorAbilities;
        break;
    case 1:
        PlayerAbilities = AbilityManager->MageAbilities;
        break;
    case 2:
        PlayerAbilities = AbilityManager->RangerAbilities;
        break;
    default:
        break;
    }
    SetupAbilityUI(PlayerAbilities);
}

void UAbilityUpgradeWidget::ConfirmAbility() {
    AbilityManager->ConfirmSelection();
}

void UAbilityUpgradeWidget::CancelSelection() {
    ClearAbilityUI();
    AbilityManager->CancelSelection();
}

void UAbilityUpgradeWidget::SetupAbilityUI(TArray<FAbilityStruct> PlayerAbilities) {
    for (int i = 0; i < PlayerAbilities.Num(); i++) {
        UAbilityWidget* AbilityWidget = CreateWidget<UAbilityWidget>(this, AbilityWidgetClass);
        AbilityWidget->InitializeUI(PlayerAbilities[i]);
        AbilitiesContainer->AddChild(AbilityWidget);
        AbilityWidgets.Add(AbilityWidget);
    }
    AbilityWidgets[0]->EnableButton();
}

void UAbilityUpgradeWidget::ClearAbilityUI() {
    AbilitiesContainer->ClearChildren();
    AbilityWidgets.Empty();
}