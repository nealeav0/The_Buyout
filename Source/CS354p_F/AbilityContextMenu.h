// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityContextMenu.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UAbilityContextMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeUI(FAbilityStruct Ability);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* NameLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MoveTypeLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TargetLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* CooldownLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PowerLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* DescriptionLabel;
};
