// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetsSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UTargetsSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeUI(TArray<FEntityStruct> EnemyStructs, UBattleHUD* BattleHUD);

	UFUNCTION(BlueprintCallable)
	void OnEnemy1Clicked();

	UFUNCTION(BlueprintCallable)
	void OnEnemy2Clicked();

	UFUNCTION(BlueprintCallable)
	void OnEnemy3Clicked();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	class UBattleHUD* ParentHUD;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Enemy1Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Enemy1Label;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Enemy2Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Enemy2Label;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Enemy3Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Enemy3Label;
};
