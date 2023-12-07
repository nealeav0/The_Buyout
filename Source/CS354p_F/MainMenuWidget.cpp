// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "MainPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/GameEngine.h"
#include "GenericPlatform/GenericPlatformMisc.h"

#define LOCTEXT_NAMESPACE "UMG"

AMainPlayerController* PC;

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // GameInstance = Cast<UMainGameInstance>(GetGameInstance());
    // if (GameInstance)
    
    StartButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnStartClicked);
    QuitButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnQuitClicked);

    PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
}

void UMainMenuWidget::InitializeStartUI()
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
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("AggieMap")), true, "");
    // remove from parent
    PC->CloseMainMenuUI();
}

void UMainMenuWidget::OnQuitClicked()
{
    PC->CloseMainMenuUI();
    PC->ConsoleCommand("quit");
}


