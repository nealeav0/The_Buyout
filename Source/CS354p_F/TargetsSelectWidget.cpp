// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetsSelectWidget.h"
#include "EntityBase.h"
#include "BattleHUD.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UTargetsSelectWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    Enemy1Button->OnClicked.AddUniqueDynamic(this, &UTargetsSelectWidget::OnEnemy1Clicked);
    Enemy2Button->OnClicked.AddUniqueDynamic(this, &UTargetsSelectWidget::OnEnemy2Clicked);
    Enemy3Button->OnClicked.AddUniqueDynamic(this, &UTargetsSelectWidget::OnEnemy3Clicked);
}

void UTargetsSelectWidget::InitializeUI(TArray<FEntityStruct> EnemyStructs, UBattleHUD *BattleHUD)
{
    ParentHUD = BattleHUD;

    if (Enemy1Label)
        Enemy1Label->SetText(FText::FromString(EnemyStructs[0].Name));
    
    if (Enemy2Label)
        Enemy2Label->SetText(FText::FromString(EnemyStructs[1].Name));

    if (Enemy3Label)
        Enemy3Label->SetText(FText::FromString(EnemyStructs[2].Name));
}

void UTargetsSelectWidget::OnEnemy1Clicked()
{
    ParentHUD->OnTargetSelected(0);
}

void UTargetsSelectWidget::OnEnemy2Clicked()
{
    ParentHUD->OnTargetSelected(1);
}

void UTargetsSelectWidget::OnEnemy3Clicked()
{
    ParentHUD->OnTargetSelected(2);
}

