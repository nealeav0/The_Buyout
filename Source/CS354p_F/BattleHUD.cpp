// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHUD.h"
#include "EntityBase.h"
#include "EntityStatsWidget.h"
#include "PartySelectWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/SlateWrapperTypes.h"
#include "MainGameInstance.h"

void UBattleHUD::NativeConstruct()
{
    Super::NativeConstruct();

    UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
    if (GameInstance)
        BattleManager = GameInstance->BattleManager();

    PartySelect->SetVisibility(ESlateVisibility::Collapsed);

    SelectButton->OnClicked.AddUniqueDynamic(this, &UBattleHUD::OnSelectClicked);
    // DefendButton->OnClicked.AddUniqueDynamic(this, &UBattleHUD::OnDefendClicked);
    EscapeButton->OnClicked.AddUniqueDynamic(this, &UBattleHUD::OnEscapeClicked);
}

void UBattleHUD::InitializeUI(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs, bool bIsPlayerTurn, TArray<FAbilityStruct> PlayerAbilities)
{
    UpdateStats(PlayerStructs, EnemyStructs);
    UpdateTurn(bIsPlayerTurn);
    PartySelect->InitializeUI(PlayerStructs, this);
    UpdateAbilities(PlayerAbilities);
}

void UBattleHUD::UpdateStats(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs)
{
    if (PlayerStats) {
        PlayerStats->InitializeStats(PlayerStructs);
    }

    if (EnemyStats) {
        EnemyStats->InitializeStats(EnemyStructs);
    }
}

void UBattleHUD::UpdateTurn(bool bIsPlayerTurn)
{
    if (TurnLabel) {
        if (bIsPlayerTurn) {
            TurnLabel->SetText(FText::FromString("PLAYER'S TURN"));
        } else {
            TurnLabel->SetText(FText::FromString("ENEMY'S TURN"));
        }
    }
}

void UBattleHUD::UpdateAbilities(TArray<FAbilityStruct> PlayerAbilities)
{
    // deprecated probably
}

void UBattleHUD::OnSelectClicked()
{
    if (PartySelect->Visibility == ESlateVisibility::Visible) 
        PartySelect->SetVisibility(ESlateVisibility::Hidden);
    else
        PartySelect->SetVisibility(ESlateVisibility::Visible);
}

void UBattleHUD::OnWarriorClicked()
{
    BattleManager->SelectPlayer(0);
    BattleManager->ConfirmSelection();
}

void UBattleHUD::OnMageClicked()
{
    BattleManager->SelectPlayer(1);
    BattleManager->ConfirmSelection();
}

void UBattleHUD::OnRangerClicked()
{
    BattleManager->SelectPlayer(2);
    BattleManager->ConfirmSelection();
}

// void UBattleHUD::OnDefendClicked()
// {
//     BattleManager->DefendHandler();
// }

void UBattleHUD::OnEscapeClicked()
{
    BattleManager->EscapeHandler();
}