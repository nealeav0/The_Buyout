// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHUD.h"
#include "EntityBase.h"
#include "EntityStatsWidget.h"
#include "PartySelectWidget.h"
#include "ActionsSelectWidget.h"
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
    ActionsSelect->SetVisibility(ESlateVisibility::Collapsed);

    SelectButton->OnClicked.AddUniqueDynamic(this, &UBattleHUD::OnSelectClicked);
    EscapeButton->OnClicked.AddUniqueDynamic(this, &UBattleHUD::OnEscapeClicked);
}

void UBattleHUD::InitializeUI(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs, bool bIsPlayerTurn, TArray<FAbilityStruct> PlayerAbilities)
{
    UpdateStats(PlayerStructs, EnemyStructs);
    UpdateTurn(bIsPlayerTurn);
    PartySelect->InitializeUI(PlayerStructs, this);
    ActionsSelect->InitializeUI(this);
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
    ActionsSelect->UpdateAbilities(PlayerAbilities);
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
    ActionsSelect->SetVisibility(ESlateVisibility::Visible);
}

void UBattleHUD::OnMageClicked()
{
    BattleManager->SelectPlayer(1);
    BattleManager->ConfirmSelection();
    ActionsSelect->SetVisibility(ESlateVisibility::Visible);
}

void UBattleHUD::OnRangerClicked()
{
    BattleManager->SelectPlayer(2);
    BattleManager->ConfirmSelection();
    ActionsSelect->SetVisibility(ESlateVisibility::Visible);
}

void UBattleHUD::OnAbilityClicked(int32 index)
{
    BattleManager->SelectAbility(index);
    BattleManager->ConfirmSelection();
}

void UBattleHUD::OnDefendClicked()
{
    // need to adjust for multiplayer maybe?
    BattleManager->DefendHandler();
}

void UBattleHUD::OnEscapeClicked()
{
    BattleManager->EscapeHandler();
}