// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionsSelectWidget.h"
#include "Ability.h"
#include "ActionButton.h"
#include "BattleHUD.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"

UActionsSelectWidget::UActionsSelectWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UActionButton> ActionButtonBPClass(TEXT("/Game/GUI/BP_ActionButton"));
    if (ActionButtonBPClass.Class)
        ActionButtonClass = ActionButtonBPClass.Class;
}

void UActionsSelectWidget::NativeConstruct()
{
    DefendButton->OnClicked.AddUniqueDynamic(this, &UActionsSelectWidget::OnDefendClicked);
}

void UActionsSelectWidget::InitializeUI(UBattleHUD *BattleHUD)
{
    ParentHUD = BattleHUD;
}

void UActionsSelectWidget::UpdateAbilities(TArray<FAbilityStruct> PlayerAbilities)
{
    // loop through player abilities and create each button
    if (AbilitiesContainer) {
        // make sure previous abilities were wiped first
        if (AbilitiesContainer->GetAllChildren().Num() > 0) {
            AbilitiesContainer->ClearChildren();
        }
        // now add abilities
        for (int i = 0; i < PlayerAbilities.Num(); i++) {
            UActionButton* ActionButton = CreateWidget<UActionButton>(this, ActionButtonClass);
            ActionButton->InitializeUI(ParentHUD);
            ActionButton->UpdateAbility(PlayerAbilities[i], i);
            AbilitiesContainer->AddChild(ActionButton);
        }
    }
}

void UActionsSelectWidget::OnDefendClicked()
{
    ParentHUD->OnDefendClicked();
}
