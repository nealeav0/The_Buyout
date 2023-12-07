// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuffStat.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UBuffStat : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeUI(FString Name, int Num, bool bIsPercent);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BuffLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PercentLabel;

};
