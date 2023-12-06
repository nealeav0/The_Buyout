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
        WarriorLabel->SetText(FText::FromString(PlayerStructs[0].EntityName));
    
    if (MageLabel)
        MageLabel->SetText(FText::FromString(PlayerStructs[1].EntityName));

    if (RangerLabel)
        RangerLabel->SetText(FText::FromString(PlayerStructs[2].EntityName));
}

void UPartySelectWidget::UpdatePlayers(TArray<FEntityStruct> PlayerStructs, TArray<int32> PlayerActions)
{
    WarriorButton->SetIsEnabled(!(PlayerStructs[0].bIsDead || PlayerActions[0] == 0));
    MageButton->SetIsEnabled(!(PlayerStructs[1].bIsDead || PlayerActions[1] == 0));
    RangerButton->SetIsEnabled(!(PlayerStructs[2].bIsDead || PlayerActions[2] == 0));
}

void UPartySelectWidget::OnWarriorClicked()
{
    ParentHUD->OnPlayerSelected(0);
}

// void UPartySelectWidget::OnWarriorReleased()
// {
//     // WarriorButton->SetStyle(&FButtonStyle::Pressed);
// }

void UPartySelectWidget::OnMageClicked()
{
    ParentHUD->OnPlayerSelected(1);
}

void UPartySelectWidget::OnRangerClicked()
{
    ParentHUD->OnPlayerSelected(2);
}
