// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionButton.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UActionButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeUI(UBattleHUD *BattleHUD);

	UFUNCTION(BlueprintCallable)
	void UpdateAbility(FAbilityStruct Ability, int32 index);

	UFUNCTION(BlueprintCallable)
	void OnButtonClicked();

protected:
	virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ActionLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LevelLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ActionButton;

	UPROPERTY()
	class UBattleHUD* ParentHUD;

	UPROPERTY()
	int32 AbilityIndex;
};
