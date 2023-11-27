// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BattleManager.h"
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

	UPROPERTY()
	UDataTable* PlayerDataTable;

	UPROPERTY()
	UDataTable* PlayerBaseDataTable;

	UPROPERTY()
	UDataTable* CommonDataTable;

	UPROPERTY()
	UDataTable* CommonBaseDataTable;

	// /Script/Engine.DataTable'/Game/Data/Player_Abilities.Player_Abilities'
	UPROPERTY()
	FSoftObjectPath PlayerDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Player_Abilities.Player_Abilities'"));

	// /Script/Engine.DataTable'/Game/Data/Player_Base_Stats.Player_Base_Stats'
	UPROPERTY()
	FSoftObjectPath PlayerBaseDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Player_Base_Stats.Player_Base_Stats'"));

	// /Script/Engine.DataTable'/Game/Data/Common_Enemy_Abilities.Common_Enemy_Abilities'
	UPROPERTY()
	FSoftObjectPath CommonDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Common_Enemy_Abilities.Common_Enemy_Abilities'"));

	// /Script/Engine.DataTable'/Game/Data/Common_Enemy_Base_Stats.Common_Enemy_Base_Stats'
	UPROPERTY()
	FSoftObjectPath CommonBaseDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Common_Enemy_Base_Stats.Common_Enemy_Base_Stats'"));

	virtual void Init() override;

	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable) UBattleManager* BattleManager();

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
