// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "EnemyBase.h"
#include "CommonEnemy.h"
#include "EvasiveEnemy.h"
#include "DefensiveEnemy.h"
#include "SupportEnemy.h"
#include "SeniorEnemy.h"
#include "DonorEnemy.h"
#include "BaronEnemy.h"
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

	TArray<FVector> PlayerPositions = { FVector(-1590, 1560, 110), FVector(-1590, 1380, 110), FVector(-1590, 1200, 110) };

	UPROPERTY()
	TSubclassOf<ACommonEnemy> CommonEnemyBP;

	UPROPERTY()
	TSubclassOf<AEvasiveEnemy> EvasiveEnemyBP;

	UPROPERTY()
	TSubclassOf<ADefensiveEnemy> DefensiveEnemyBP;

	UPROPERTY()
	TSubclassOf<ASupportEnemy> SupportEnemyBP;

	UPROPERTY()
	TSubclassOf<ASeniorEnemy> SeniorEnemyBP;

	UPROPERTY()
	TSubclassOf<ADonorEnemy> DonorEnemyBP;

	UPROPERTY()
	TSubclassOf<ABaronEnemy> BaronEnemyBP;

	TArray<FEntityStruct> Enemies;

	TArray<AEnemyBase*> EnemyReferences;

	TArray<FVector> EnemyPositions = { FVector(250, 350, 5), FVector(100, 350, 5), FVector(-50, 350, 5) };

	TArray<int32> PlayerActions;

	class UMainGameInstance* GameInstance;

	bool bPlayerTurn = true;

	bool bBattleEnd = false;

	int PlayerIndex = 0; // Q and E to cycle through players. Spacebar to confirm.

	int AbilityIndex = 0; // Q and E to cycle through abilities. Spacebar to confirm.

	int TargetIndex = 0; // Q and E to cycle through enemies. Spacebar to confirm.

	bool bSelectingPlayer = true;

	void SelectPlayer(int32 index);

	bool bSelectingAbility = false;

	void SelectAbility(int32 index);

	bool bSelectingTarget = false;

	void SelectTarget(int32 index);

	void ConfirmSelection();

	void CancelSelection();

	void DefendHandler();

	void EscapeHandler();

	FTimerHandle TransitionTimer;

	UFUNCTION() 
	void LeaveBattle();

	UFUNCTION() 
	void LoadBattle();

	UFUNCTION() 
	void StartBattle();

	void PrepareForBattle(TArray<FEntityStruct> NewPlayers, TArray<FEntityStruct> NewEnemies);

	void InitializeEnemyStats(FEntityStruct& Enemy);

	void SpawnEnemies();

	UFUNCTION(BlueprintCallable) 
	FEntityStruct GetPlayer();

	UFUNCTION(BlueprintCallable) 
	FEntityStruct GetEnemy();

	void SetPlayerAbility();

	void SetEnemyTarget();

	void HandlePlayerInput(FAbilityStruct SelectedAbility);

	void HandleEnemyInput(FEntityStruct Source, FAbilityStruct SelectedAbility);

	void AdjustCooldowns();

	void HandleAttack(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target);

	void HandleStatus(EStatusTypeEnum Status, float StatusChance, float StatusPower, FEntityStruct& Target);

	void AdjustBuffs(FEntityStruct& Target);

	void AdjustStatus(FEntityStruct& Target);

	void HandleMagic(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target);

	void HandleHealing(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target);

	void HandleBurnDamage(FEntityStruct& Target);

	void HandlePoisonDamage(FEntityStruct& Target);

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
