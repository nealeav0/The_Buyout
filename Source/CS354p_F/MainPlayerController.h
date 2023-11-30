// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainPlayerController();

	/* --- MAIN MENU UI --- */

    UFUNCTION()
	void OpenMainMenuUI();

	UFUNCTION()
	void CloseMainMenuUI();

	/* --- PAUSE MENU UI --- */

	UFUNCTION()
	void OpenPauseMenuUI();

    UFUNCTION()
	void ClosePauseMenuUI();

	// UFUNCTION()
	// void PauseGame();

	UFUNCTION()
	void ResumeGame();

	/* --- BATTLE UI --- */

    UFUNCTION()
	void OpenBattleUI();

	UFUNCTION()
	void CloseBattleUI();

	UFUNCTION()
	void InitUI(FEntityStruct PlayerStruct, FEntityStruct EnemyStruct, bool bIsPlayerTurn, TArray<FAbilityStruct> PlayerAbilities);

	UFUNCTION()
	void UpdateBattleStats(FEntityStruct PlayerStruct, FEntityStruct EnemyStruct);

	UFUNCTION()
	void UpdateTurnUI(bool bIsPlayerTurn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = true))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = true))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = true))
	class UInputAction* MoveCameraAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = true))
	class UInputAction* NavigateAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = true))
	class UInputAction* ConfirmAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = true))
	class UInputAction* PauseAction;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Events
	void OnMovePressed(const FInputActionValue& Value);
	void OnCameraMoved(const FInputActionValue& Value);
	void OnNavigatePressed(const FInputActionValue& Value);
	void OnConfirmPressed();
    void OnPausePressed(const FInputActionValue &Value);

	UFUNCTION()
	void UpdateInputMode(UUserWidget* WidgetToFocus, bool bEnableCursor);

    /* Battle HUD */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UBattleWidget> BattleWidgetClass;

	UPROPERTY()
	class UBattleWidget* BattleWidget;

	/* Main Menu */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY()
	class UMainMenuWidget* MainMenuWidget;

	/* Pause Menu */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY()
	class UPauseMenuWidget* PauseMenuWidget;
};
