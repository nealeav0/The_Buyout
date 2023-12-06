// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHUD.h"
#include "EntityBase.h"
#include "EntityStatsWidget.h"
#include "PartySelectWidget.h"
#include "ActionsSelectWidget.h"
#include "TargetsSelectWidget.h"
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
    TargetsSelect->SetVisibility(ESlateVisibility::Collapsed);

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
        PartySelect->SetVisibility(ESlateVisibility::Collapsed);
    else
        PartySelect->SetVisibility(ESlateVisibility::Visible);
}

void UBattleHUD::OnPlayerSelected(int32 index)
{
    BattleManager->SelectPlayer(index);
    BattleManager->ConfirmSelection();
    ActionsSelect->SetVisibility(ESlateVisibility::Visible);
}

void UBattleHUD::OnAbilitySelected(int32 index)
{
    BattleManager->SelectAbility(index);
    BattleManager->ConfirmSelection();
    TargetsSelect->SetVisibility(ESlateVisibility::Visible);
}

void UBattleHUD::OnTargetSelected(int32 index)
{
    BattleManager->SelectTarget(index);
    BattleManager->ConfirmSelection();
    ActionsSelect->SetVisibility(ESlateVisibility::Collapsed);
    TargetsSelect->SetVisibility(ESlateVisibility::Collapsed);
}

void UBattleHUD::OnDefendClicked()
{
    BattleManager->DefendHandler();
    ActionsSelect->SetVisibility(ESlateVisibility::Collapsed);
}

void UBattleHUD::OnEscapeClicked()
{
    BattleManager->EscapeHandler();
}