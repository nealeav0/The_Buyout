// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"
#include "MainPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/GameEngine.h"
#include "MainGameInstance.h"
#include "GenericPlatform/GenericPlatformMisc.h"

#define LOCTEXT_NAMESPACE "UMG"

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    ResumeButton->OnClicked.AddUniqueDynamic(this, &UPauseMenuWidget::OnResumeClicked);
    AudioButton->OnClicked.AddUniqueDynamic(this, &UPauseMenuWidget::OnAudioClicked);
    QuitButton->OnClicked.AddUniqueDynamic(this, &UPauseMenuWidget::OnQuitClicked);

    PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
}

void UPauseMenuWidget::InitializePauseUI()
{
    if (ResumeLabel) {
        ResumeLabel->SetText(FText::FromString("RESUME GAME"));
    }

    if (AudioLabel) {
        AudioLabel->SetText(FText::FromString("AUDIO ON/OFF"));
    }

    if (QuitLabel) {
        QuitLabel->SetText(FText::FromString("QUIT GAME"));
    }
}

void UPauseMenuWidget::OnResumeClicked()
{
    // unpause game state
    PC->ResumeGame();
    // remove from parent
    PC->ClosePauseMenuUI();
}

void UPauseMenuWidget::OnAudioClicked()
{
    UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance) {
        // toggle audio
        GameInstance->ToggleMute();
    }
}

void UPauseMenuWidget::OnQuitClicked()
{
    PC->ClosePauseMenuUI();
    PC->ConsoleCommand("quit");
}


