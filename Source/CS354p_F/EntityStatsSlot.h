// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EntityStatsSlot.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UEntityStatsSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeStats(FEntityStruct EntityStruct);

	UFUNCTION(BlueprintCallable)
	void UpdateHealth(FEntityStruct EntityStruct);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* NameLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LevelLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HealthLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;
};
