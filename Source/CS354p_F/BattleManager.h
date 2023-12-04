// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "EnemyBase.h"
#include "CommonEnemy.h"
#include "EntityBase.h"
#include "UObject/NoExportTypes.h"
#include "BattleManager.generated.h"

UCLASS()
class CS354P_F_API UBattleManager : public UObject
{
	GENERATED_BODY()
	
public:
	UBattleManager();

	TArray<FEntityStruct> Players;

	TArray<FEntityStruct> Enemies;

	TArray<int32> PlayerActions;

	class UMainGameInstance* GameInstance;

	bool bPlayerTurn = true;

	bool bBattleEnd = false;

	int PlayerIndex = 0; // Q and E to cycle through players. Spacebar to confirm.

	int AbilityIndex = 0; // Q and E to cycle through abilities. Spacebar to confirm.

	int TargetIndex = 0; // Q and E to cycle through enemies. Spacebar to confirm.

	bool bSelectingPlayer;

	void SelectPlayer(float Navigation);

	bool bSelectingAbility;

	void SelectAbility(float Navigation);

	bool bSelectingTarget;

	void SelectTarget(float Navigation);

	void ConfirmSelection();

	void DefendHandler();

	void EscapeHandler();

	FTimerHandle TransitionTimer;

	UFUNCTION() 
	void LeaveBattle();

	UFUNCTION() 
	void LoadBattle();

	UFUNCTION() 
	void StartBattle();

	void PrepareForBattle(TArray<FEntityStruct> NewPlayers, FEntityStruct EnemyStruct);

	UFUNCTION(BlueprintCallable) 
	FEntityStruct GetPlayer();

	UFUNCTION(BlueprintCallable) 
	FEntityStruct GetEnemy();

	void SetPlayerAbility();

	void SetEnemyTarget();

	void HandlePlayerInput(FAbilityStruct SelectedAbility);

	void HandleEnemyInput(FAbilityStruct SelectedAbility);

	void AdjustCooldowns();

	void HandleAttack(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target);

	void HandleStatus(EStatusTypeEnum Status, float StatusChance, float StatusPower, FEntityStruct& Target);

	void AdjustBuffs(FEntityStruct& Target);

	void AdjustStatus(FEntityStruct& Target);

	void HandleMagic(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target);

	void HandleHealing(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target);

	void HandleBurnDamage(FEntityStruct& Target);

	// Round scripting

	void StartRound();

	void EndRound();

	void PlayerToEnemyTransition();

	void EnemyToPlayerTransition();

	void PlayerTurn();

	void EnemyTurn();
	
	bool CheckPlayersIsDead();

	bool CheckEnemiesIsDead();

	void Die(FEntityStruct& Target);

	UPROPERTY()
	int32 TotalEXP = 0;

	UPROPERTY()
	int32 TotalAP = 0;

	void HandleEXP();

};
