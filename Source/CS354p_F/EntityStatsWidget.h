// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EntityStatsWidget.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UEntityStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeStats(TArray<FEntityStruct> EntityStructs);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* StatsContainer;
};
