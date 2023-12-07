// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityContextMenu.h"
#include "Ability.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "UMG"

void UAbilityContextMenu::NativeConstruct()
{
    Super::NativeConstruct();
}

void UAbilityContextMenu::InitializeUI(FAbilityStruct Ability)
{
    if (NameLabel)
        NameLabel->SetText(FText::FromString(Ability.AbilityName));

    if (MoveTypeLabel) {
        FFormatNamedArguments Args;
        switch (Ability.MoveType) {
            case EMoveTypeEnum::ATTACK :
                Args.Add(TEXT("MoveType"), FText::FromString(TEXT("Attack")));
                break;
            case EMoveTypeEnum::MAGIC :
                Args.Add(TEXT("MoveType"), FText::FromString(TEXT("Magic")));
                break;
            case EMoveTypeEnum::HEAL :
                Args.Add(TEXT("MoveType"), FText::FromString(TEXT("Heal")));
                break;
        }

        switch (Ability.ElementType) {
            case EElementTypeEnum::FIRE :
                Args.Add(TEXT("ElementType"), FText::FromString(TEXT("Fire")));
                break;
            case EElementTypeEnum::ICE :
                Args.Add(TEXT("ElementType"), FText::FromString(TEXT("Ice")));
                break;
            case EElementTypeEnum::THUNDER :
                Args.Add(TEXT("ElementType"), FText::FromString(TEXT("Thunder")));
                break;
            case EElementTypeEnum::POISON :
                Args.Add(TEXT("ElementType"), FText::FromString(TEXT("Poison")));
                break;
            case EElementTypeEnum::NONELEMENTAL :
                Args.Add(TEXT("ElementType"), FText::FromString(TEXT("Non-Elemental")));
                break;
        }
        MoveTypeLabel->SetText(FText::Format(LOCTEXT("MoveTypeLabel", "{MoveType} ({ElementType})"), Args));
    }
    
    if (TargetLabel) {
        switch (Ability.TargetType) {
            case ETargetTypeEnum::ALLY :
                TargetLabel->SetText(FText::FromString(TEXT("Single Ally")));
                break;
            case ETargetTypeEnum::ALLIES :
                TargetLabel->SetText(FText::FromString(TEXT("All Allies")));
                break;
            case ETargetTypeEnum::SINGLE :
                TargetLabel->SetText(FText::FromString(TEXT("Single Enemy")));
                break;
            case ETargetTypeEnum::ALL :
                TargetLabel->SetText(FText::FromString(TEXT("All Enemies")));
                break;
            case ETargetTypeEnum::RANDOM :
                TargetLabel->SetText(FText::FromString(TEXT("Random Enemy")));
                break;
        }
    }

    if (CooldownLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("MaxCooldown"), FText::AsNumber(Ability.MaxCooldown));
        CooldownLabel->SetText(FText::Format(LOCTEXT("CooldownLabel", "Cooldown: {MaxCooldown}"), Args));
    }

    if (PowerLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("CurrPower"), FText::AsNumber(Ability.Power[Ability.Level - 1]));
        PowerLabel->SetText(FText::Format(LOCTEXT("PowerLabel", "Current Power: {CurrPower}"), Args));
    }

    if (DescriptionLabel)
        DescriptionLabel->SetText(FText::FromString(Ability.Description));
}


