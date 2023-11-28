// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "MainCharacter.h"
#include "MainGameInstance.h"
#include "BattleGameModeBase.h"
#include "BattleWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BattleManager.h"
#include "MainGameInstance.h"

ABattleGameModeBase* BattleMode;

AMainPlayerController::AMainPlayerController()
{
	BattleWidgetClass = nullptr;
	BattleWidget = nullptr;
}

void AMainPlayerController::OpenBattleUI()
{
	if (BattleWidgetClass) {
		BattleWidget = CreateWidget<UBattleWidget>(this, BattleWidgetClass);
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

void AMainPlayerController::UpdateBattleStats(FPlayerStruct PlayerStruct, FEnemyStruct EnemyStruct)
{
	if (BattleWidget)
		BattleWidget->UpdateStats(PlayerStruct, EnemyStruct);
}

void AMainPlayerController::UpdateTurnUI(bool bIsPlayerTurn)
{
	if (BattleWidget) 
		BattleWidget->UpdateTurn(bIsPlayerTurn);
}

void AMainPlayerController::InitUI(FPlayerStruct PlayerStruct, FEnemyStruct EnemyStruct, bool bIsPlayerTurn, TArray<FAbilityStruct> PlayerAbilities)
{
	if (BattleWidget)
		BattleWidget->InitializeUI(PlayerStruct, EnemyStruct, bIsPlayerTurn, PlayerAbilities);
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	BattleMode = Cast<ABattleGameModeBase>(GetWorld()->GetAuthGameMode());

	if (BattleMode) {
		OpenBattleUI();
		
		// set up the click-only input behavior 
		FInputModeUIOnly InputMode;
		// if we want to later lock the mouse cursor (can't move outside the PIE), we can change this
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		if (BattleWidget)
			InputMode.SetWidgetToFocus(BattleWidget->TakeWidget());
		SetInputMode(InputMode);

		// let's also show the cursor
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
	} else {
		// set up no click behavior
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(false);
		SetInputMode(InputMode);
		
		// also hide cursor
		bShowMouseCursor = false;
		bEnableClickEvents = false;
		bEnableMouseOverEvents = false;
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	BattleMode = Cast<ABattleGameModeBase>(GetWorld()->GetAuthGameMode());

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// in BattleMode we don't want to move
		// so only bind these if we're in the overworld
		if (!BattleMode) { 
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::OnMovePressed);
			EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &AMainPlayerController::OnCameraMoved);
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

