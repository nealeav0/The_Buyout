// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class CS354P_F_API UBattleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateStats(FPlayerStruct PlayerStruct, FEnemyStruct EnemyStruct);

	UFUNCTION(BlueprintCallable)
	void UpdateTurn(bool bIsPlayerTurn);

	UFUNCTION(BlueprintCallable)
	void UpdateAbilities(TArray<FAbilityStruct> PlayerAbilities);

    UFUNCTION(BlueprintCallable)
	void InitializeUI(FPlayerStruct PlayerStruct, FEnemyStruct EnemyStruct, bool bIsPlayerTurn, TArray<FAbilityStruct> PlayerAbilities);

	UFUNCTION(BlueprintCallable)
	void OnDefendClicked();

	UFUNCTION(BlueprintCallable)
	void OnEscapeClicked();

	UFUNCTION(BlueprintCallable)
	void OnAbility1Clicked();

	UFUNCTION(BlueprintCallable)
	void OnAbility2Clicked();

	UFUNCTION(BlueprintCallable)
	void OnAbility3Clicked();

	UFUNCTION(BlueprintCallable)
	void OnAbility4Clicked();

	UFUNCTION(BlueprintCallable)
	void OnAbility5Clicked();

	UFUNCTION(BlueprintCallable)
	void OnAbility6Clicked();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PlayerLabel;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PlayerHealthLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PAttackLabel;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PDefenseLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PEvasionLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PAccuracyLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* EnemyLabel;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* EnemyHealthLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* EAttackLabel;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* EDefenseLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* EEvasionLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* EAccuracyLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TurnLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* DefendButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* DefendButtonLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* EscapeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* EscapeButtonLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* AbilityContainer;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Ability1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AbilityLabel1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Ability2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AbilityLabel2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Ability3;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AbilityLabel3;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Ability4;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AbilityLabel4;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Ability5;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AbilityLabel5;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Ability6;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AbilityLabel6;

};
