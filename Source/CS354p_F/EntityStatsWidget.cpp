// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityStatsWidget.h"
#include "EntityBase.h"
#include "EntityStatsSlot.h"
#include "Components/VerticalBox.h"

void UEntityStatsWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UEntityStatsWidget::InitializeStats(TArray<FEntityStruct> EntityStructs)
{
    if (StatsContainer) {
        TArray<UWidget*> Slots = StatsContainer->GetAllChildren(); 
        for (int i = 0; i < EntityStructs.Num(); i++) {
            UEntityStatsSlot* EntityStatSlot = Cast<UEntityStatsSlot>(Slots[i]);
            EntityStatSlot->InitializeStats(EntityStructs[i]);
        }
    }
}

