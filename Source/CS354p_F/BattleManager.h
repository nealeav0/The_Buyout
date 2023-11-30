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

	TArray<int32> PlayerActions = { 1 };

	//FTurnDelegate TurnDelegate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle DataHandle = FDataTableRowHandle();

	class UMainGameInstance* GameInstance;

	int Rounds = 0;

	bool bPlayerTurn = true;

	bool bBattleEnd = false;

	FAbilityStruct LoadedAbility;

	FEnemyStruct LoadedEnemyTarget;

	bool bAbilityChosen = false;

	bool bTargetChosen = false;

	int AbilityIndex = 0;

	void StartTurn();

	/*void AttackHandler(FAbilityStruct Ability, FEnemyStruct &Target);

	void AttackHandler(FAbilityStruct Ability, FPlayerStruct &Target);*/

	void DefendHandler();

	void EscapeHandler();

	void EndTurn();

	FTimerHandle TransitionTimer;

	UFUNCTION() void LeaveBattle();

	UFUNCTION() void LoadBattle();

	UFUNCTION() void StartBattle();

	void PrepareForBattle(FEntityStruct PlayerStruct, FEntityStruct EnemyStruct);

	UFUNCTION(BlueprintCallable) 
	FEntityStruct GetPlayer();

	UFUNCTION(BlueprintCallable) 
	FEntityStruct GetEnemy();

	void SetPlayerAbility();

	void SetEnemyTarget();

	void HandlePlayerInput(FAbilityStruct SelectedAbility);

	void HandleEnemyInput(FAbilityStruct SelectedAbility);

	/*void HandleStatus(EStatusTypeEnum Status, float StatusPower, FPlayerStruct &Target);

	void HandleStatus(EStatusTypeEnum Status, float StatusPower, FEnemyStruct &Target);

	void HandleMagic(FAbilityStruct Ability, FPlayerStruct& Target);

	void HandleMagic(FAbilityStruct Ability, FEnemyStruct& Target);

	void HandleHealing(FAbilityStruct Ability, FPlayerStruct &Target);

	void HandleHealing(FAbilityStruct Ability, FEnemyStruct &Target);*/

	/*void AdjustBuffs(FPlayerStruct &Target);

	void AdjustBuffs(FEnemyStruct &Target);*/

	void AdjustCooldowns();

	void HandleAttack(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target);

	void HandleStatus(EStatusTypeEnum Status, float StatusChance, float StatusPower, FEntityStruct& Target);

	void AdjustBuffs(FEntityStruct& Target);

	void HandleMagic(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target);

	void HandleHealing(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target);

	void HandleBurnDamage(FEntityStruct& Target);

	// Round scripting

	void StartRound(); //Perhaps we could have a counter in each array to represent the number of actions a player can take [1, 1, 1]

	void EndRound();

	void PlayerToEnemyTransition();

	void EnemyToPlayerTransition();

	UPROPERTY()
	float TotalEXP = 0.f;

	UPROPERTY()
	float EXPThreshold = 10.f;

	void HandleEXP();

};
