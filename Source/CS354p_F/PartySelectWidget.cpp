// Fill out your copyright notice in the Description page of Project Settings.


#include "PartySelectWidget.h"
#include "EntityBase.h"
#include "BattleHUD.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Styling/SlateTypes.h"

void UPartySelectWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    WarriorButton->OnClicked.AddUniqueDynamic(this, &UPartySelectWidget::OnWarriorClicked);
    // WarriorButton->OnReleased.AddUniqueDynamic(this, &UPartySelectWidget::OnWarriorReleased);
    MageButton->OnClicked.AddUniqueDynamic(this, &UPartySelectWidget::OnMageClicked);
    RangerButton->OnClicked.AddUniqueDynamic(this, &UPartySelectWidget::OnRangerClicked);
}

void UPartySelectWidget::InitializeUI(TArray<FEntityStruct> PlayerStructs, UBattleHUD* BattleHUD)
{
    ParentHUD = BattleHUD;

    if (WarriorLabel)
        WarriorLabel->SetText(FText::FromString(PlayerStructs[0].Name));
    
    if (MageLabel)
        MageLabel->SetText(FText::FromString(PlayerStructs[1].Name));

    if (RangerLabel)
        RangerLabel->SetText(FText::FromString(PlayerStructs[2].Name));
}

void UPartySelectWidget::OnWarriorClicked()
{
    ParentHUD->OnWarriorClicked();
}

// void UPartySelectWidget::OnWarriorReleased()
// {
//     // WarriorButton->SetStyle(&FButtonStyle::Pressed);
// }

void UPartySelectWidget::OnMageClicked()
{
    ParentHUD->OnMageClicked();
}

void UPartySelectWidget::OnRangerClicked()
{
    ParentHUD->OnRangerClicked();
}
