// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetButton.h"
#include "BattleHUD.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UTargetButton::NativeConstruct()
{
    TargetButton->OnClicked.AddUniqueDynamic(this, &UTargetButton::OnButtonClicked);
}

void UTargetButton::InitializeUI(UBattleHUD* BattleHUD)
{
    ParentHUD = BattleHUD;
}

void UTargetButton::UpdateTarget(FString Name, int32 index)
{
    if (TargetLabel) {
        TargetLabel->SetText(FText::FromString(Name));
    }

    TargetIndex = index;
}

void UTargetButton::OnButtonClicked()
{
    ParentHUD->OnTargetSelected(TargetIndex);
}