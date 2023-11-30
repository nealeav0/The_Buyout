// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "EnemyBase.h"
#include "CommonEnemy.h"
#include "EntityBase.h"
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

	TArray<FEntityStruct> Players;

	TArray<FEntityStruct> Enemies;

	TArray<int32> PlayerActions;

	//FTurnDelegate TurnDelegate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle DataHandle = FDataTableRowHandle();

	int Rounds = 0;

	bool bPlayerTurn = true;

	bool bBattleEnd = false;

	//FAbilityStruct LoadedAbility;

	/*FEnemyStruct LoadedEnemyTarget;*/

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

	void StartRound(); //Perhaps we could have a counter in each array to represent the number of actions a player can take [1, 1, 1]

	void EndRound();

	void PlayerToEnemyTransition();

	void EnemyToPlayerTransition();

	void PlayerTurn();

	void EnemyTurn();
	
	bool CheckPlayersIsDead();

	bool CheckEnemiesIsDead();

	void Die(FEntityStruct& Target);

	UPROPERTY()
	float TotalEXP = 0.f;

	void HandleEXP();

};
