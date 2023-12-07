// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuffsWidget.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UBuffsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBuffsWidget(const FObjectInitializer &ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void InitializeUI(FEntityStruct Entity);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHorizontalBox* BuffsContainer;

	UPROPERTY()
	TSubclassOf<class UBuffStat> BuffStatClass;
};
