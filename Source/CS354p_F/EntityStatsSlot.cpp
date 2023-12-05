// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityStatsSlot.h"
#include "EntityBase.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#define LOCTEXT_NAMESPACE "UMG"

void UEntityStatsSlot::NativeConstruct()
{
    Super::NativeConstruct();
}

void UEntityStatsSlot::InitializeStats(FEntityStruct EntityStruct)
{
    if (NameLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Name"), FText::FromString(EntityStruct.Name));
        NameLabel->SetText(FText::Format(LOCTEXT("NameLabel", "{Name}"), Args));
    }

    if (LevelLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Level"), FText::AsNumber(EntityStruct.Level));
        LevelLabel->SetText(FText::Format(LOCTEXT("PlayerLabel", "(Lv. {Level})"), Args));
    }

    UpdateHealth(EntityStruct);
}

void UEntityStatsSlot::UpdateHealth(FEntityStruct EntityStruct)
{
     if (HealthLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Current"), FText::AsNumber(FMath::RoundToInt(EntityStruct.Health)));
        Args.Add(TEXT("Max"), FText::AsNumber(FMath::RoundToInt(EntityStruct.MaxHealth)));
        HealthLabel->SetText(FText::Format(LOCTEXT("HealthLabel", "{Current} / {Max}"), Args));
    }

    if (HealthBar) {
        HealthBar->SetPercent(EntityStruct.Health / EntityStruct.MaxHealth);
    }
}
