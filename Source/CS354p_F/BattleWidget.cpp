// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleWidget.h"
#include "MainCharacter.h"
#include "EnemyBase.h"
#include "MainGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"

#define LOCTEXT_NAMESPACE "UMG"

void UBattleWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GameInstance = Cast<UMainGameInstance>(GetGameInstance());
    if (GameInstance)
        BattleManager = GameInstance->BattleManager();

    DefendButton->OnClicked.AddUniqueDynamic(this, &UBattleWidget::OnDefendClicked);
    EscapeButton->OnClicked.AddUniqueDynamic(this, &UBattleWidget::OnEscapeClicked);
    Ability1->OnClicked.AddUniqueDynamic(this, &UBattleWidget::OnAbility1Clicked);
    Ability2->OnClicked.AddUniqueDynamic(this, &UBattleWidget::OnAbility2Clicked);
    Ability3->OnClicked.AddUniqueDynamic(this, &UBattleWidget::OnAbility3Clicked);
    Ability4->OnClicked.AddUniqueDynamic(this, &UBattleWidget::OnAbility4Clicked);
    Ability5->OnClicked.AddUniqueDynamic(this, &UBattleWidget::OnAbility5Clicked);
    Ability6->OnClicked.AddUniqueDynamic(this, &UBattleWidget::OnAbility6Clicked);
}

void UBattleWidget::UpdateStats(FEntityStruct PlayerStruct, FEntityStruct EnemyStruct)
{
    /* -- Player Stats -- */
    if (PlayerLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Level"), FText::AsNumber(PlayerStruct.Level));
        PlayerLabel->SetText(FText::Format(LOCTEXT("PlayerLabel", "PLAYER (Lv. {Level})"), Args));
    }

    if (PlayerHealthLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Current"), FText::AsNumber(FMath::RoundToInt(PlayerStruct.Health)));
        Args.Add(TEXT("Max"), FText::AsNumber(FMath::RoundToInt(PlayerStruct.MaxHealth)));
        PlayerHealthLabel->SetText(FText::Format(LOCTEXT("PlayerHealthLabel", "Health: {Current} / {Max}"), Args));
    }

    if (PAttackLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Percent"), FText::AsNumber(PlayerStruct.AttackBuff * 100));
        PAttackLabel->SetText(FText::Format(LOCTEXT("PAttackLabel", "Attack: {Percent}%"), Args));
    }

    if (PDefenseLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Percent"), FText::AsNumber(PlayerStruct.DefenseBuff * 100));
        PDefenseLabel->SetText(FText::Format(LOCTEXT("PDefenseLabel", "Defense: {Percent}%"), Args));
    }

    if (PEvasionLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Percent"), FText::AsNumber(PlayerStruct.EvasionBuff * 100));
        PEvasionLabel->SetText(FText::Format(LOCTEXT("PEvasionLabel", "Evasion: {Percent}%"), Args));
    }
    
    if (PAccuracyLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Percent"), FText::AsNumber(PlayerStruct.AccuracyBuff * 100));
        PAccuracyLabel->SetText(FText::Format(LOCTEXT("PAccuracyLabel", "Accuracy: {Percent}%"), Args));
    }

    /* -- Enemy Stats -- */
    if (EnemyLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Level"), FText::AsNumber(EnemyStruct.Level));
        EnemyLabel->SetText(FText::Format(LOCTEXT("EnemyLabel", "COMMON ENEMY (Lv. {Level})"), Args));
    }

    if (EnemyHealthLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Current"), FText::AsNumber(FMath::RoundToInt(EnemyStruct.Health)));
        Args.Add(TEXT("Max"), FText::AsNumber(FMath::RoundToInt(EnemyStruct.MaxHealth)));
        EnemyHealthLabel->SetText(FText::Format(LOCTEXT("EnemyHealthLabel", "Health: {Current} / {Max}"), Args));
    }

    if (EAttackLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Percent"), FText::AsNumber(EnemyStruct.AttackBuff * 100));
        EAttackLabel->SetText(FText::Format(LOCTEXT("EAttackLabel", "Attack: {Percent}%"), Args));
    }

    if (EDefenseLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Percent"), FText::AsNumber(EnemyStruct.DefenseBuff * 100));
        EDefenseLabel->SetText(FText::Format(LOCTEXT("EDefenseLabel", "Defense: {Percent}%"), Args));
    }

    if (EEvasionLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Percent"), FText::AsNumber(EnemyStruct.EvasionBuff * 100));
        EEvasionLabel->SetText(FText::Format(LOCTEXT("EEvasionLabel", "Evasion: {Percent}%"), Args));
    }
    
    if (EAccuracyLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Percent"), FText::AsNumber(EnemyStruct.AccuracyBuff * 100));
        EAccuracyLabel->SetText(FText::Format(LOCTEXT("EAccuracyLabel", "Accuracy: {Percent}%"), Args));
    }
}

void UBattleWidget::UpdateTurn(bool bIsPlayerTurn)
{
    if (TurnLabel) {
        if (bIsPlayerTurn) {
            TurnLabel->SetText(FText::FromString("PLAYER'S TURN"));
        } else {
            TurnLabel->SetText(FText::FromString("ENEMY'S TURN"));
        }
    }
}

void UBattleWidget::UpdateAbilities(TArray<FAbilityStruct> PlayerAbilities)
{
    if (AbilityContainer)
    {
        int32 index = 0;
        for (UWidget* Button : AbilityContainer->GetAllChildren()) {
            UButton* AbilityButton = Cast<UButton>(Button);
            if (AbilityButton) {
                for (UWidget* ButtonLabel : AbilityButton->GetAllChildren()) {
                    UTextBlock* AbilityLabel = Cast<UTextBlock>(ButtonLabel);
                    if (AbilityLabel) {
                        if (PlayerAbilities.IsValidIndex(index))
                        {
                            AbilityLabel->SetText(FText::FromString(PlayerAbilities[index].AbilityName));
                            index++;
                        }
                        else
                        {
                            if (GEngine)
                            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("The abilities do not exist.")));
                        }
                    }
                }
            }
        }
    }
}

void UBattleWidget::InitializeUI(FEntityStruct PlayerStruct, FEntityStruct EnemyStruct, bool bIsPlayerTurn, TArray<FAbilityStruct> PlayerAbilities)
{
    UpdateStats(PlayerStruct, EnemyStruct);
    UpdateTurn(bIsPlayerTurn);
    UpdateAbilities(PlayerAbilities);
}

void UBattleWidget::OnDefendClicked()
{
    BattleManager->DefendHandler();
}

void UBattleWidget::OnEscapeClicked()
{
    BattleManager->EscapeHandler();
}

void UBattleWidget::OnAbility1Clicked()
{
    BattleManager->PlayerIndex = 0;
    BattleManager->AbilityIndex = 0;
    BattleManager->TargetIndex = 0;
    BattleManager->bSelectingPlayer = false;
    BattleManager->bSelectingAbility = false;
    BattleManager->bSelectingTarget = false;
    BattleManager->SetPlayerAbility();
}

void UBattleWidget::OnAbility2Clicked()
{
    BattleManager->PlayerIndex = 0;
    BattleManager->AbilityIndex = 1;
    BattleManager->TargetIndex = 0;
    BattleManager->bSelectingPlayer = false;
    BattleManager->bSelectingAbility = false;
    BattleManager->bSelectingTarget = false;
    BattleManager->SetPlayerAbility();
}

void UBattleWidget::OnAbility3Clicked()
{
    BattleManager->PlayerIndex = 0;
    BattleManager->AbilityIndex = 2;
    BattleManager->TargetIndex = 0;
    BattleManager->bSelectingPlayer = false;
    BattleManager->bSelectingAbility = false;
    BattleManager->bSelectingTarget = false;
    BattleManager->SetPlayerAbility();
}

void UBattleWidget::OnAbility4Clicked()
{
    BattleManager->PlayerIndex = 0;
    BattleManager->AbilityIndex = 3;
    BattleManager->TargetIndex = 0;
    BattleManager->bSelectingPlayer = false;
    BattleManager->bSelectingAbility = false;
    BattleManager->bSelectingTarget = false;
    BattleManager->SetPlayerAbility();
}

void UBattleWidget::OnAbility5Clicked()
{
    BattleManager->PlayerIndex = 0;
    BattleManager->AbilityIndex = 4;
    BattleManager->TargetIndex = 0;
    BattleManager->bSelectingPlayer = false;
    BattleManager->bSelectingAbility = false;
    BattleManager->bSelectingTarget = false;
    BattleManager->SetPlayerAbility();
}

void UBattleWidget::OnAbility6Clicked()
{
    BattleManager->PlayerIndex = 0;
    BattleManager->AbilityIndex = 5;
    BattleManager->TargetIndex = 0;
    BattleManager->bSelectingPlayer = false;
    BattleManager->bSelectingAbility = false;
    BattleManager->bSelectingTarget = false;
    BattleManager->SetPlayerAbility();
}
