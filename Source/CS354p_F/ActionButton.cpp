// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionButton.h"
#include "Ability.h"
#include "Components/Widget.h"
#include "BattleHUD.h"
#include "AbilityContextMenu.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#define LOCTEXT_NAMESPACE "UMG"

UActionButton::UActionButton(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UAbilityContextMenu> AbilityMenuBPClass(TEXT("/Game/GUI/BP_AbilityContextMenu"));
    if (AbilityMenuBPClass.Class)
        AbilityMenuClass = AbilityMenuBPClass.Class;
}

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

    if (AbilityMenuClass) {
        AbilityMenuWidget = CreateWidget<UAbilityContextMenu>(this, AbilityMenuClass);
        if (AbilityMenuWidget) {
            AbilityMenuWidget->InitializeUI(Ability);
            SetToolTip(AbilityMenuWidget);
        }
    }

    ActionButton->SetIsEnabled(!(Ability.Cooldown > 0));
    AbilityIndex = index;
}

void UActionButton::OnButtonClicked()
{
    ParentHUD->OnAbilitySelected(AbilityIndex);
}