// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BattleManager.h"
#include "AbilityManager.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	UBattleManager* BattleManagerInstance;

	UPROPERTY(Transient)
	UAbilityManager* AbilityManagerInstance;

	UPROPERTY()
	UDataTable* PlayerAbilityDataTable;

	UPROPERTY()
	UDataTable* PlayerBaseDataTable;

	UPROPERTY()
	UDataTable* EnemyAbilityDataTable;

	UPROPERTY()
	UDataTable* EnemyBaseDataTable;

	// /Script/Engine.DataTable'/Game/Data/Player_Abilities.Player_Abilities'
	UPROPERTY()
	FSoftObjectPath PlayerAbilityDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Player_Abilities.Player_Abilities'"));

	// /Script/Engine.DataTable'/Game/Data/Player_Base_Stats.Player_Base_Stats'
	UPROPERTY()
	FSoftObjectPath PlayerBaseDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Player_Base_Stats.Player_Base_Stats'"));

	// /Script/Engine.DataTable'/Game/Data/Common_Enemy_Abilities.Common_Enemy_Abilities'
	UPROPERTY()
	FSoftObjectPath EnemyAbilityDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Common_Enemy_Abilities.Common_Enemy_Abilities'"));

	// /Script/Engine.DataTable'/Game/Data/Enemy_Base_Stats.Enemy_Base_Stats'
	UPROPERTY()
	FSoftObjectPath EnemyBaseDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Enemy_Base_Stats.Enemy_Base_Stats'"));

	virtual void Init() override;

	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable) 
	UBattleManager* BattleManager();

	UFUNCTION(BlueprintCallable)
	UAbilityManager* AbilityManager();

	void UpdateBattleManager();

	UPROPERTY()
	TArray<ACommonEnemy*> Enemies;

	UPROPERTY()
	FVector PlayerLastSavedLocation;

	UFUNCTION()
	void SetPlayerLastLocation(FVector Location);

	UFUNCTION()
	FVector GetPlayerLastLocation();

	// void DestroyCurrentEnemy();
};
