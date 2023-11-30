// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializePauseUI();

	UFUNCTION(BlueprintCallable)
	void OnResumeClicked();

	UFUNCTION(BlueprintCallable)
	void OnQuitClicked();

protected:

	class AMainPlayerController* PC;
	
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ResumeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ResumeLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* QuitLabel;
};
