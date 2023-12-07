// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityUpgradeWidget.h"
#include "EntityBase.h"
#include "BattleHUD.h"
#include "AbilityWidget.h"
#include "AbilityManager.h"
#include "MainPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"

void UAbilityUpgradeWidget::InitializeUI(UAbilityManager* AbilityManagerIn) {

    AbilityManager = AbilityManagerIn;

    TArray<FEntityStruct> PlayerStructs = AbilityManager->GetPlayersArray();
    if (WarriorLabel)
        WarriorLabel->SetText(FText::FromString(PlayerStructs[0].EntityName));

    if (MageLabel)
        MageLabel->SetText(FText::FromString(PlayerStructs[1].EntityName));

    if (RangerLabel)
        RangerLabel->SetText(FText::FromString(PlayerStructs[2].EntityName));


    WarriorButton->OnClicked.AddUniqueDynamic(this, &UAbilityUpgradeWidget::OnWarriorClicked);
    MageButton->OnClicked.AddUniqueDynamic(this, &UAbilityUpgradeWidget::OnMageClicked);
    RangerButton->OnClicked.AddUniqueDynamic(this, &UAbilityUpgradeWidget::OnRangerClicked);

    /*if (WarriorButton)
        WarriorButton->SetIsEnabled(true);

    if (MageButton)
        MageButton->SetIsEnabled(false);

    if (RangerButton)
        RangerButton->SetIsEnabled(false);*/

    /*if (AbilityPointButton)
        AbilityPointButton->SetVisibility(ESlateVisibility::Hidden);*/

    ConfirmUpgradeMenu->SetVisibility(ESlateVisibility::Hidden);
    SelectedAbility = -1;
    UpgradeButton->OnClicked.AddUniqueDynamic(this, &UAbilityUpgradeWidget::OnUpgradeClicked);
    ExitButton->OnClicked.AddUniqueDynamic(this, &UAbilityUpgradeWidget::OnExitClicked);

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
    FEntityStruct SelectedPlayer = AbilityManager->GetPlayersArray()[AbilityManager->PlayerIndex];
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
    /*FAbilityStruct SelectedAbility = PlayerAbilities[AbilityManager->AbilityIndex];
    if (SelectedAbility.Level >= SelectedAbility.MaxLevel) {
        AbilityPointLabel->SetText(FText::FromString(TEXT("Ability At Max Level")));
    }
    else {
        AbilityPointLabel->SetText(FText::FromString(FString::Printf(TEXT("Ability Points: %d/%d"), SelectedPlayer.AbilityPoints, SelectedAbility.APCosts[SelectedAbility.Level])));
    }*/
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
        AbilityWidget->InitializeUI(PlayerAbilities[i],i,this);
        AbilitiesContainer->AddChild(AbilityWidget);
        AbilityWidgets.Add(AbilityWidget);
    }
    //AbilityWidgets[0]->EnableButton();
    /*AbilityPointButton->SetVisibility(ESlateVisibility::Visible);*/
    /*FEntityStruct SelectedPlayer = AbilityManager->GetPlayersArray()[AbilityManager->PlayerIndex];
    AbilityPointLabel->SetText(FText::FromString(FString::Printf(TEXT("Ability Points: %d/%d"), SelectedPlayer.AbilityPoints, PlayerAbilities[0].APCosts[PlayerAbilities[0].Level])));*/
}

void UAbilityUpgradeWidget::ClearAbilityUI() {
    AbilitiesContainer->ClearChildren();
    AbilityWidgets.Empty();
    /*AbilityPointButton->SetVisibility(ESlateVisibility::Hidden);*/
}

void UAbilityUpgradeWidget::OnWarriorClicked() {
    ClearAbilityUI();
    SetupAbilityUI(AbilityManager->WarriorAbilities);
    AbilityManager->PlayerIndex = 0;
    AbilityManager->AbilityIndex = 0;
    AbilityManager->bSelectingPlayer = false;
    AbilityManager->bSelectingAbility = true;
    ConfirmUpgradeMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UAbilityUpgradeWidget::OnMageClicked() {
    ClearAbilityUI();
    SetupAbilityUI(AbilityManager->MageAbilities);
    AbilityManager->PlayerIndex = 1;
    AbilityManager->AbilityIndex = 0;
    AbilityManager->bSelectingPlayer = false;
    AbilityManager->bSelectingAbility = true;
    ConfirmUpgradeMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UAbilityUpgradeWidget::OnRangerClicked() {
    ClearAbilityUI();
    SetupAbilityUI(AbilityManager->RangerAbilities);
    AbilityManager->PlayerIndex = 2;
    AbilityManager->AbilityIndex = 0;
    AbilityManager->bSelectingPlayer = false;
    AbilityManager->bSelectingAbility = true;
    ConfirmUpgradeMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UAbilityUpgradeWidget::OnAbilitySelect(int AbilityIndex) {
    ConfirmUpgradeMenu->SetVisibility(ESlateVisibility::Visible);
    SelectedAbility = AbilityIndex;

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
    FAbilityStruct Ability = PlayerAbilities[AbilityIndex];
    FEntityStruct SelectedPlayer = AbilityManager->GetPlayersArray()[AbilityManager->PlayerIndex];
    if (Ability.Level >= Ability.MaxLevel) {
        AbilityPointLabel->SetText(FText::FromString(TEXT("Ability At Max Level")));
    }
    else
    {
        AbilityPointLabel->SetText(FText::FromString(FString::Printf(TEXT("Ability Points: %d/%d"), SelectedPlayer.AbilityPoints, Ability.APCosts[Ability.Level])));
    }
}

void UAbilityUpgradeWidget::OnUpgradeClicked() {
    if (SelectedAbility > -1) {
        int OldIndex = AbilityManager->PlayerIndex;
        AbilityManager->AbilityIndex = SelectedAbility;
        AbilityManager->ConfirmSelection();
        switch (OldIndex)
        {
        case 0:
            OnWarriorClicked();
            break;
        case 1:
            OnMageClicked();
            break;
        case 2:
            OnRangerClicked();
            break;
        default:
            break;
        }
    }
}

void UAbilityUpgradeWidget::OnExitClicked() {
    AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
    if (PC) {
        // unpause game state
        PC->ResumeGame();
        // remove from parent
        PC->CloseAbilityUpgradeUI();
    }
}