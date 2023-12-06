// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "MainCharacter.h"
#include "MainGameInstance.h"
#include "BattleGameModeBase.h"
#include "BattleHUD.h"
#include "MainMenuGameModeBase.h"
#include "MainMenuWidget.h"
#include "PauseMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BattleManager.h"
#include "MainGameInstance.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

ABattleGameModeBase* BattleMode;
AMainMenuGameModeBase* MainMenuMode;

AMainPlayerController::AMainPlayerController()
{
	BattleWidgetClass = nullptr;
	BattleWidget = nullptr;
	MainMenuWidgetClass = nullptr;
	MainMenuWidget = nullptr;
	PauseMenuWidgetClass = nullptr;
	PauseMenuWidget = nullptr;
}

void AMainPlayerController::SwitchMeshMaterial(int32 index)
{
	AMainCharacter* MC = Cast<AMainCharacter>(GetPawn());
	MC->SetMaterial(index);
}

/* --- MAIN MENU UI --- */

void AMainPlayerController::OpenMainMenuUI() {
	if (MainMenuWidgetClass) {
		MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);
		MainMenuWidget->AddToPlayerScreen();
		
		if (MainMenuWidget)
			MainMenuWidget->InitializeStartUI();
	}
}

void AMainPlayerController::CloseMainMenuUI() {
	if (MainMenuWidgetClass) {
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}
}

/* --- PAUSE MENU UI --- */

void AMainPlayerController::OpenPauseMenuUI()
{
	if (PauseMenuWidgetClass) {
		PauseMenuWidget = CreateWidget<UPauseMenuWidget>(this, PauseMenuWidgetClass);
		PauseMenuWidget->AddToPlayerScreen();
		
		if (PauseMenuWidget)
			PauseMenuWidget->InitializePauseUI();
	}
}

void AMainPlayerController::ClosePauseMenuUI()
{
	if (PauseMenuWidgetClass) {
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;
	}
}

void AMainPlayerController::ResumeGame()
{
	this->SetPause(false);
	UpdateInputMode(nullptr, false);
}

/* --- BATTLE UI --- */

void AMainPlayerController::OpenBattleUI()
{
	if (BattleWidgetClass) {
		BattleWidget = CreateWidget<UBattleHUD>(this, BattleWidgetClass);
		BattleWidget->AddToPlayerScreen();

		// let battlemanager know everything's loaded in !!
		UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
		if (GameInstance)
			GameInstance->BattleManager()->StartBattle();
	}
}

void AMainPlayerController::CloseBattleUI()
{
	if (BattleWidgetClass) {
		BattleWidget->RemoveFromParent();
		BattleWidget = nullptr;
	}
}

void AMainPlayerController::InitUI(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs, bool bIsPlayerTurn, TArray<FAbilityStruct> PlayerAbilities)
{
	if (BattleWidget)
		BattleWidget->InitializeUI(PlayerStructs, EnemyStructs, bIsPlayerTurn, PlayerAbilities);
}

void AMainPlayerController::UpdateBattleStats(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs)
{
	if (BattleWidget)
		BattleWidget->UpdateStats(PlayerStructs, EnemyStructs);
}

void AMainPlayerController::UpdateTurnUI(bool bIsPlayerTurn)
{
	if (BattleWidget) 
		BattleWidget->UpdateTurn(bIsPlayerTurn);
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	BattleMode = Cast<ABattleGameModeBase>(GetWorld()->GetAuthGameMode());
	MainMenuMode = Cast<AMainMenuGameModeBase>(GetWorld()->GetAuthGameMode());
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());

	if (BattleMode) {
		GameInstance->BGMusic->SetPaused(true);
		GameInstance->PlayBattleAudio();

		OpenBattleUI();
		// set up the click-only input behavior 
		UpdateInputMode(BattleWidget, true); // needs to be udated to true later

	} else if (MainMenuMode) { // main/starting menu
		GameInstance->PlayBGAudio();
			
		OpenMainMenuUI();
		// set up the click-only input behavior 
		UpdateInputMode(MainMenuWidget, true);

	} else { // we're in the overworld
		if (GameInstance->BGMusic->bIsPaused)
			GameInstance->BGMusic->SetPaused(false);
		
		// set up no click behavior
		UpdateInputMode(nullptr, false);
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	BattleMode = Cast<ABattleGameModeBase>(GetWorld()->GetAuthGameMode());
	MainMenuMode = Cast<AMainMenuGameModeBase>(GetWorld()->GetAuthGameMode());

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// in BattleMode we don't want to move
		// so only bind these if we're in the overworld
		if (!(BattleMode || MainMenuMode)) { 
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::OnMovePressed);
			EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &AMainPlayerController::OnCameraMoved);
			EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AMainPlayerController::OnPausePressed);
			EnhancedInputComponent->BindAction(NavigateAction, ETriggerEvent::Triggered, this, &AMainPlayerController::OnNavigatePressed);
			EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Triggered, this, &AMainPlayerController::OnConfirmPressed);
			EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Triggered, this, &AMainPlayerController::OnCancelPressed);
		} 
		if (BattleMode)
		{
			EnhancedInputComponent->BindAction(NavigateAction, ETriggerEvent::Triggered, this, &AMainPlayerController::OnNavigatePressed);
			EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Triggered, this, & AMainPlayerController::OnConfirmPressed);
			EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Triggered, this, &AMainPlayerController::OnCancelPressed);
		}
	}
} 

void AMainPlayerController::OnMovePressed(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AMainCharacter* character = Cast<AMainCharacter>(this->GetCharacter());
	if (character)
		character->MoveEvent(MovementVector);
}


void AMainPlayerController::OnCameraMoved(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AMainCharacter* character = Cast<AMainCharacter>(this->GetCharacter());
	if (character)
		character->MoveCameraEvent(MovementVector);
}

void AMainPlayerController::OnNavigatePressed(const FInputActionValue& Value)
{
	// float Navigation = Value.Get<float>();
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("%f navigate."), Navigation));
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		if (BattleMode)
		{
			if (GameInstance->BattleManager()->bPlayerTurn)
			{
				if (GameInstance->BattleManager()->bSelectingPlayer)
				{
					// GameInstance->BattleManager()->SelectPlayer(Navigation);
				}
				else if (GameInstance->BattleManager()->bSelectingAbility)
				{
					// GameInstance->BattleManager()->SelectAbility(Navigation);
				}
				else if (GameInstance->BattleManager()->bSelectingTarget)
				{
					// GameInstance->BattleManager()->SelectTarget(Navigation);
				}
			}
		}
		else
		{
			if (GameInstance->AbilityManager()->bSelectingPlayer)
			{
				// GameInstance->AbilityManager()->SelectPlayer(Navigation);
			}
			else if (GameInstance->AbilityManager()->bSelectingAbility)
			{
				// GameInstance->AbilityManager()->SelectAbility(Navigation);
			}
		}
	}
}

void AMainPlayerController::OnConfirmPressed()
{
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		if (BattleMode)
		{
			GameInstance->BattleManager()->ConfirmSelection();
		}
		else
		{
			GameInstance->AbilityManager()->ConfirmSelection();
		}
	}
}

void AMainPlayerController::OnCancelPressed()
{
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		if (BattleMode)
		{
			GameInstance->BattleManager()->CancelSelection();
		}
		else
		{
			GameInstance->AbilityManager()->CancelSelection();
		}
	}
}

void AMainPlayerController::OnPausePressed(const FInputActionValue& Value)
{
	OpenPauseMenuUI();
	this->SetPause(true);
	if (PauseMenuWidget)
		UpdateInputMode(PauseMenuWidget, true);
}

void AMainPlayerController::UpdateInputMode(UUserWidget* WidgetToFocus, bool bEnableCursor)
{
	// update input mode
	if (bEnableCursor) { // this means UI only
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		if (WidgetToFocus)
			InputMode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
		SetInputMode(InputMode);
	} else { // for now
		// update input mode to game
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(false);
		SetInputMode(InputMode);
	}

	// show/hide cursor
	bShowMouseCursor = bEnableCursor;
	bEnableClickEvents = bEnableCursor;
	bEnableMouseOverEvents = bEnableCursor;
}
