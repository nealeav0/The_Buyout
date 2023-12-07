// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffStat.h"
#include "EntityBase.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "UMG"

void UBuffStat::NativeConstruct()
{
    Super::NativeConstruct();
}

void UBuffStat::InitializeUI(FString Name, int Num, bool bIsPercent)
{
    if (BuffLabel) {
        BuffLabel->SetText(FText::FromString(Name));
    }
    
    if (PercentLabel) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("Percent"), FText::AsNumber(Num));
        if (bIsPercent) {
            PercentLabel->SetText(FText::Format(LOCTEXT("PercentLabel", "{Percent}%"), Args));
        } else {
            PercentLabel->SetText(FText::Format(LOCTEXT("PercentLabel", "{Percent}"), Args));
        }
    }
}
