// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "EnemyBase.h"
#include "CommonEnemy.h"
#include "UObject/NoExportTypes.h"
#include "BattleManager.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnDelegate);
/**
 * 
 */
UCLASS()
class CS354P_F_API UBattleManager : public UObject
{
	GENERATED_BODY()
	
public:
	UBattleManager();

	TArray<FPlayerStruct> Players;

	TArray<FEnemyStruct> Enemies;

	//FTurnDelegate TurnDelegate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle DataHandle = FDataTableRowHandle();

	int Rounds = 0;

	bool bPlayerTurn = true;

	bool bBattleEnd = false;

	FAbilityStruct LoadedAbility;

	FEnemyStruct LoadedEnemyTarget;

	bool bAbilityChosen = false;

	bool bTargetChosen = false;

	int AbilityIndex = 0;

	void StartTurn();

	void AttackHandler(FAbilityStruct Ability, FEnemyStruct &Target);

	void AttackHandler(FAbilityStruct Ability, FPlayerStruct &Target);

	void DefendHandler();

	void EscapeHandler();

	void EndTurn();

	FTimerHandle TransitionTimer;

	UFUNCTION() void LeaveBattle();

	UFUNCTION() void LoadBattle();

	UFUNCTION() void StartBattle();

	void PrepareForBattle(FPlayerStruct PlayerStruct, FEnemyStruct EnemyStruct);

	UFUNCTION(BlueprintCallable) FPlayerStruct GetPlayer();

	UFUNCTION(BlueprintCallable) FEnemyStruct GetEnemy();

	void SetPlayerAbility();

	void SetEnemyTarget();

	void HandlePlayerInput(FAbilityStruct SelectedAbility);

	void HandleEnemyInput(FAbilityStruct SelectedAbility);

	void HandleStatus(EStatusTypeEnum Status, float StatusPower, FPlayerStruct &Target);

	void HandleStatus(EStatusTypeEnum Status, float StatusPower, FEnemyStruct &Target);

	void HandleMagic(FAbilityStruct Ability, FPlayerStruct& Target);

	void HandleMagic(FAbilityStruct Ability, FEnemyStruct& Target);

	void HandleHealing(FAbilityStruct Ability, FPlayerStruct &Target);

	void HandleHealing(FAbilityStruct Ability, FEnemyStruct &Target);

	void AdjustBuffs(FPlayerStruct &Target);

	void AdjustBuffs(FEnemyStruct &Target);

	void AdjustCooldowns();

	UPROPERTY()
	float TotalEXP = 0.f;

	UPROPERTY()
	float EXPThreshold = 10.f;

	void HandleEXP();

};
