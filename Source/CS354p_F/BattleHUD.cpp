// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHUD.h"
#include "EntityBase.h"
#include "EntityStatsWidget.h"
#include "PartySelectWidget.h"
#include "ActionsSelectWidget.h"
#include "TargetsSelectWidget.h"
#include "BuffsWidget.h"
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
    UpdateBuffs(PlayerStructs, EnemyStructs);
    UpdateTurn(bIsPlayerTurn);
    PartySelect->InitializeUI(PlayerStructs, this);
    ActionsSelect->InitializeUI(this);
    TargetsSelect->InitializeUI(this);
}

void UBattleHUD::UpdateStats(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs)
{
    if (PlayerStats) {
        PlayerStats->InitializeStats(PlayerStructs);
    }

    if (EnemyStats) {
        EnemyStats->InitializeStats(EnemyStructs);
    }

    UpdateBuffs(PlayerStructs, EnemyStructs);
}

void UBattleHUD::UpdateBuffs(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs)
{
    Enemy1Buffs->InitializeUI(EnemyStructs[0]);
    Enemy2Buffs->InitializeUI(EnemyStructs[1]);
    Enemy3Buffs->InitializeUI(EnemyStructs[2]);

    PlayerBuffs->InitializeUI(PlayerStructs[0]);
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
    // update players menu every time round ends would be good
    UpdatePlayers(BattleManager->Players, BattleManager->PlayerActions);
}

void UBattleHUD::UpdatePlayers(TArray<FEntityStruct> PlayerStructs, TArray<int32> PlayerActions)
{
    PartySelect->UpdatePlayers(PlayerStructs, PlayerActions);
}

void UBattleHUD::UpdateAbilities(TArray<FAbilityStruct> PlayerAbilities)
{
    ActionsSelect->UpdateAbilities(PlayerAbilities);
}

void UBattleHUD::UpdateTargets(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs, ETargetTypeEnum TargetType)
{
    TargetsSelect->UpdateTargets(PlayerStructs, EnemyStructs, TargetType);
}

void UBattleHUD::OnSelectClicked()
{
    if (PartySelect->Visibility == ESlateVisibility::Visible) {
        PartySelect->SetVisibility(ESlateVisibility::Collapsed);
        ActionsSelect->SetVisibility(ESlateVisibility::Collapsed);
        TargetsSelect->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
        PartySelect->SetVisibility(ESlateVisibility::Visible);
}

void UBattleHUD::OnPlayerSelected(int32 index)
{
    BattleManager->SelectPlayer(index);
    UpdateAbilities(BattleManager->GetPlayer().Abilities);
    PlayerBuffs->InitializeUI(BattleManager->GetPlayer());
    ActionsSelect->SetVisibility(ESlateVisibility::Visible);
    TargetsSelect->SetVisibility(ESlateVisibility::Collapsed);
}

void UBattleHUD::OnAbilitySelected(int32 index)
{
    BattleManager->SelectAbility(index);
    ETargetTypeEnum ETargetType = BattleManager->GetPlayer().Abilities[index].TargetType;
    if (ETargetType == ETargetTypeEnum::ALLY || ETargetType == ETargetTypeEnum::SINGLE ) {
        UpdateTargets(BattleManager->Players, BattleManager->Enemies, ETargetType);
        TargetsSelect->SetVisibility(ESlateVisibility::Visible);
    } else {
        OnTargetSelected(0);
    }
}

void UBattleHUD::OnTargetSelected(int32 index)
{
    BattleManager->SelectTarget(index);
    BattleManager->ConfirmSelection();
    ActionsSelect->SetVisibility(ESlateVisibility::Collapsed);
    TargetsSelect->SetVisibility(ESlateVisibility::Collapsed);
    UpdatePlayers(BattleManager->Players, BattleManager->PlayerActions);
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