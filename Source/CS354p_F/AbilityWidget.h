// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityWidget.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UAbilityWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void InitializeUI(FAbilityStruct Ability);

	UFUNCTION()
	void EnableButton();

	UFUNCTION()
	void DisableButton();
	
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ActionLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LevelLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ActionButton;
};
