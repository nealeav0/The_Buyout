// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetButton.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UTargetButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeUI(UBattleHUD *BattleHUD);

	UFUNCTION(BlueprintCallable)
	void UpdateTarget(FEntityStruct TargetStruct, int32 index);

	UFUNCTION(BlueprintCallable)
	void OnButtonClicked();

protected:
	virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TargetLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* TargetButton;

	UPROPERTY()
	class UBattleHUD* ParentHUD;

	UPROPERTY()
	int32 TargetIndex;
};
