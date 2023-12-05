// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartySelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UPartySelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeUI(TArray<FEntityStruct> PlayerStructs, UBattleHUD* BattleHUD);

	UFUNCTION(BlueprintCallable)
	void OnWarriorClicked();

	// UFUNCTION(BlueprintCallable)
	// void OnWarriorReleased();

	UFUNCTION(BlueprintCallable)
	void OnMageClicked();

	UFUNCTION(BlueprintCallable)
	void OnRangerClicked();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	class UBattleHUD* ParentHUD;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* WarriorButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* WarriorLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MageButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MageLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* RangerButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* RangerLabel;
};
