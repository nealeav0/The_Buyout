// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionsSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UActionsSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UActionsSelectWidget(const FObjectInitializer &ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void InitializeUI(UBattleHUD *BattleHUD);

	UFUNCTION(BlueprintCallable)
	void UpdateAbilities(TArray<FAbilityStruct> PlayerAbilities);

	UFUNCTION(BlueprintCallable)
	void OnDefendClicked();

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* AbilitiesContainer;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* DefendButton;

	UPROPERTY()
	class UBattleHUD* ParentHUD;

	UPROPERTY()
	TSubclassOf<class UActionButton> ActionButtonClass;
};
