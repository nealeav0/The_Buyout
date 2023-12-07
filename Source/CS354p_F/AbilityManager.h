// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Ability.h"
#include "MainCharacter.h"
#include "AbilityManager.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UAbilityManager : public UObject
{
	GENERATED_BODY()
	
public:

	int PlayerIndex = 0;

	UPROPERTY()
	TArray<FEntityStruct> Players;

	int AbilityIndex = 0;

	bool bSelectingPlayer = true;

	bool bSelectingAbility = false;

	UPROPERTY()
	UDataTable* PlayerAbilityDataTable = nullptr;

	UPROPERTY()
	TArray<FAbilityStruct> WarriorAbilities;

	UPROPERTY()
	UDataTable* MageAbilityDataTable = nullptr;

	UPROPERTY()
	TArray<FAbilityStruct> MageAbilities;

	UPROPERTY()
	UDataTable* RangerAbilityDataTable = nullptr;
	
	UPROPERTY()
	TArray<FAbilityStruct> RangerAbilities;

	UPROPERTY()
	UDataTable* EnemyAbilityDataTable = nullptr;

	// /Script/Engine.DataTable'/Game/Data/Player_Abilities.Player_Abilities'
	UPROPERTY()
	FSoftObjectPath PlayerAbilityDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Player_Abilities.Player_Abilities'"));

	// /Script/Engine.DataTable'/Game/Data/Mage_Abilities.Mage_Abilities'
	UPROPERTY()
	FSoftObjectPath MageAbilityDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Mage_Abilities.Mage_Abilities'"));

	// /Script/Engine.DataTable'/Game/Data/Ranger_Abilities.Ranger_Abilities'
	UPROPERTY()
	FSoftObjectPath RangerAbilityDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Ranger_Abilities.Ranger_Abilities'"));

	// /Script/Engine.DataTable'/Game/Data/Enemy_Abilities.Enemy_Abilities'
	UPROPERTY()
	FSoftObjectPath EnemyAbilityDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Enemy_Abilities.Enemy_Abilities'"));

	UFUNCTION()
	void InitializeAbilityDataTables();

	UFUNCTION()
	void InitializePlayerArray(TArray<FEntityStruct> NewPlayers);

	UFUNCTION()
	TArray<FEntityStruct> GetPlayersArray();

	UFUNCTION(BlueprintCallable)
	void InitializeAbilities(FEntityStruct& Entity);

	UFUNCTION()
	void SelectPlayer(float Navigation);

	UFUNCTION()
	void SelectAbility(float Navigation);

	UFUNCTION()
	void ConfirmSelection();

	UFUNCTION()
	void CancelSelection();

	UFUNCTION(BlueprintCallable)
	void LearnAbility(FEntityStruct& Player, FAbilityStruct& Ability);

	UFUNCTION(BlueprintCallable)
	void UpgradeAbility(FEntityStruct& Player, FAbilityStruct& Ability);
};
