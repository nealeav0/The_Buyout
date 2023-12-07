// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleHUD.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UBattleHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeUI(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs, bool bIsPlayerTurn, TArray<FAbilityStruct> PlayerAbilities);

	UFUNCTION(BlueprintCallable)
	void UpdateStats(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs);

	UFUNCTION(BlueprintCallable)
	void UpdateBuffs(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs);

	UFUNCTION(BlueprintCallable)
	void UpdateTurn(bool bIsPlayerTurn);

	UFUNCTION(BlueprintCallable)
	void UpdatePlayers(TArray<FEntityStruct> PlayerStructs, TArray<int32> PlayerActions);

	UFUNCTION(BlueprintCallable)
	void UpdateAbilities(TArray<FAbilityStruct> PlayerAbilities);

    UFUNCTION(BlueprintCallable)
	void UpdateTargets(TArray<FEntityStruct> PlayerStructs, TArray<FEntityStruct> EnemyStructs, ETargetTypeEnum TargetType);

    UFUNCTION(BlueprintCallable)
	void OnSelectClicked();

	UFUNCTION(BlueprintCallable)
	void OnPlayerSelected(int32 index);

    UFUNCTION(BlueprintCallable)
	void OnAbilitySelected(int32 index);

	UFUNCTION(BlueprintCallable)
	void OnTargetSelected(int32 index);

    UFUNCTION(BlueprintCallable)
	void OnDefendClicked();

	UFUNCTION(BlueprintCallable)
	void OnEscapeClicked();

protected:
	virtual void NativeConstruct() override;
	
	class UBattleManager* BattleManager;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TurnLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEntityStatsWidget* PlayerStats;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEntityStatsWidget* EnemyStats;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* SelectButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UPartySelectWidget* PartySelect;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UActionsSelectWidget* ActionsSelect;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTargetsSelectWidget* TargetsSelect;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* EscapeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBuffsWidget* Enemy1Buffs;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBuffsWidget* Enemy2Buffs;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBuffsWidget* Enemy3Buffs;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBuffsWidget* PlayerBuffs;
};
