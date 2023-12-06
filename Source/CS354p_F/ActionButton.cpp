// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionButton.h"
#include "Ability.h"
#include "BattleHUD.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#define LOCTEXT_NAMESPACE "UMG"

void UActionButton::NativeConstruct()
{
    ActionButton->OnClicked.AddUniqueDynamic(this, &UActionButton::OnButtonClicked);
}

void UActionButton::InitializeUI(UBattleHUD* BattleHUD)
{
    ParentHUD = BattleHUD;
}

void UActionButton::UpdateAbility(FAbilityStruct Ability, int32 index)
{
    if (ActionLabel)
        ActionLabel->SetText(FText::FromString(Ability.AbilityName));

    if (LevelLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Level"), FText::AsNumber(Ability.Level));
        LevelLabel->SetText(FText::Format(LOCTEXT("LevelLabel", "(Lv. {Level})"), Args));
    }

    AbilityIndex = index;
}

void UActionButton::OnButtonClicked()
{
    ParentHUD->OnAbilitySelected(AbilityIndex);
}
