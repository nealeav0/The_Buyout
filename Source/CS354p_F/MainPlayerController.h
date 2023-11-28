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

    UFUNCTION()
	void OpenMainMenuUI();

    UFUNCTION()
	void OpenBattleUI();

	UFUNCTION()
	void CloseBattleUI();

	UFUNCTION()
	void UpdateBattleStats(FPlayerStruct PlayerStruct, FEnemyStruct EnemyStruct);

	UFUNCTION()
	void UpdateTurnUI(bool bIsPlayerTurn);

	UFUNCTION()
	void InitUI(FPlayerStruct PlayerStruct, FEnemyStruct EnemyStruct, bool bIsPlayerTurn, TArray<FAbilityStruct> PlayerAbilities);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = true))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = true))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = true))
	class UInputAction* MoveCameraAction;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Events
	void OnMovePressed(const FInputActionValue& Value);
	void OnCameraMoved(const FInputActionValue& Value);

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
};
