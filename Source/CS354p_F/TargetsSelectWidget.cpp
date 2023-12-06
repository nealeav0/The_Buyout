// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetsSelectWidget.h"
#include "Ability.h"
#include "EntityBase.h"
#include "BattleHUD.h"
#include "TargetButton.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

UTargetsSelectWidget::UTargetsSelectWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UTargetButton> TargetButtonBPClass(TEXT("/Game/GUI/BP_TargetButton"));
    if (TargetButtonBPClass.Class)
        TargetButtonClass = TargetButtonBPClass.Class;
}

void UTargetsSelectWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UTargetsSelectWidget::InitializeUI(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs, UBattleHUD *BattleHUD)
{
    Players = PlayerStructs;
    Enemies = EnemyStructs;
    ParentHUD = BattleHUD;
}

void UTargetsSelectWidget::UpdateTargets(ETargetTypeEnum TargetType)
{
    if (TargetsContainer) {
        // make sure any previous targets were wiped first
        if (TargetsContainer->GetAllChildren().Num() > 0) {
            TargetsContainer->ClearChildren();
        }
        // now add targets
        switch (TargetType)
        {
            case ETargetTypeEnum::ALLY:
                // we need to have 3 buttons for the party members
                for (int i = 0; i < Players.Num(); i++) {
                    UTargetButton* TargetButton = CreateWidget<UTargetButton>(this, TargetButtonClass);
                    TargetButton->InitializeUI(ParentHUD);
                    TargetButton->UpdateTarget(Players[i].Name, i);
                    TargetsContainer->AddChild(TargetButton);
                }
                break;
            case ETargetTypeEnum::SINGLE:
                // we need to have 3 buttons for the enemies
                for (int i = 0; i < Enemies.Num(); i++) {
                    UTargetButton* TargetButton = CreateWidget<UTargetButton>(this, TargetButtonClass);
                    TargetButton->InitializeUI(ParentHUD);
                    TargetButton->UpdateTarget(Enemies[i].Name, i);
                    TargetsContainer->AddChild(TargetButton);
                }
                break;
            default:
                // we don't need buttons for the other types (they shouldn't even make it to this stage)
                break;
        }
    }
}
