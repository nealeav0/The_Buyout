// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "MainGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformMisc.h"

#define LOCTEXT_NAMESPACE "UMG"

// UMainGameInstance* GameInstance;

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // GameInstance = Cast<UMainGameInstance>(GetGameInstance());
    // if (GameInstance)
    
    StartButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnStartClicked);
    QuitButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnQuitClicked);
}

void UMainMenuWidget::InitializeUI()
{
    if (StartLabel) {
        StartLabel->SetText(FText::FromString("START GAME"));
    }

    if (QuitLabel) {
        QuitLabel->SetText(FText::FromString("QUIT GAME"));
    }
}

void UMainMenuWidget::OnStartClicked()
{
    // open overworld level
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("OverworldMap")), true, "");
    // remove from parent
    if (GetParent())
        RemoveFromParent();
}

void UMainMenuWidget::OnQuitClicked()
{
    if (GetParent())
        RemoveFromParent();
    // close (execute console command quit)
    FGenericPlatformMisc::RequestExit(false);
}


