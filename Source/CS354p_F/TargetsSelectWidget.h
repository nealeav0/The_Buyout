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
	UTargetsSelectWidget(const FObjectInitializer &ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void InitializeUI(UBattleHUD* BattleHUD);

	UFUNCTION(BlueprintCallable)
	void UpdateTargets(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs, ETargetTypeEnum TargetType);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* TargetsContainer;

	UPROPERTY()
	class UBattleHUD* ParentHUD;

	UPROPERTY()
	TSubclassOf<class UTargetButton> TargetButtonClass;
};
