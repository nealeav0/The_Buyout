// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHUD.h"
#include "EntityBase.h"
#include "EntityStatsWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "MainGameInstance.h"

void UBattleHUD::NativeConstruct()
{
    Super::NativeConstruct();

    UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
    if (GameInstance)
        BattleManager = GameInstance->BattleManager();

    DefendButton->OnClicked.AddUniqueDynamic(this, &UBattleHUD::OnDefendClicked);
    EscapeButton->OnClicked.AddUniqueDynamic(this, &UBattleHUD::OnEscapeClicked);
    AbilitiesButton->OnClicked.AddUniqueDynamic(this, &UBattleHUD::OnAbilitiesClicked);
}

void UBattleHUD::InitializeUI(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs, bool bIsPlayerTurn, TArray<FAbilityStruct> PlayerAbilities)
{
    UpdateStats(PlayerStructs, EnemyStructs);
    UpdateTurn(bIsPlayerTurn);
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

void UBattleHUD::OnAbilitiesClicked()
{
    // spawn menu of abilities !
}

void UBattleHUD::OnDefendClicked()
{
    BattleManager->DefendHandler();
}

void UBattleHUD::OnEscapeClicked()
{
    BattleManager->EscapeHandler();
}