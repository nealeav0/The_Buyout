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

	// /Script/Engine.DataTable'/Game/Data/Player_Abilities.Player_Abilities'
	UPROPERTY()
	FSoftObjectPath PlayerAbilityDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Player_Abilities.Player_Abilities'"));

	// /Script/Engine.DataTable'/Game/Data/Mage_Abilities.Mage_Abilities'
	UPROPERTY()
	FSoftObjectPath MageAbilityDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Mage_Abilities.Mage_Abilities'"));

	UFUNCTION()
	void InitializeAbilityDataTables();

	UFUNCTION()
	void InitializePlayerArray(TArray<FEntityStruct> NewPlayers);

	UFUNCTION()
	TArray<FEntityStruct> GetPlayersArray();

	UFUNCTION(BlueprintCallable)
	void InitializeAbilities(FEntityStruct& Player);

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
