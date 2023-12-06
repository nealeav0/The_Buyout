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
	UDataTable* PlayerBaseDataTable;

	UPROPERTY()
	UDataTable* EnemyBaseDataTable;

	UPROPERTY(EditAnywhere)
	UDataTable* DialogueDataTable;

	// /Script/Engine.DataTable'/Game/Data/Player_Base_Stats.Player_Base_Stats'
	UPROPERTY()
	FSoftObjectPath PlayerBaseDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Player_Base_Stats.Player_Base_Stats'"));

	// /Script/Engine.DataTable'/Game/Data/Enemy_Base_Stats.Enemy_Base_Stats'
	UPROPERTY()
	FSoftObjectPath EnemyBaseDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Enemy_Base_Stats.Enemy_Base_Stats'"));

	// /Script/Engine.DataTable'/Game/Data/Dialogue.Dialogue'
	UPROPERTY(VisibleAnywhere)
	FSoftObjectPath DialogueDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Dialogue.Dialogue'"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACommonEnemy> CommonEnemyBPClass;

	virtual void Init() override;

	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable) 
	UBattleManager* BattleManager();

	UFUNCTION(BlueprintCallable)
	UAbilityManager* AbilityManager();

	void UpdateBattleManager();

	UFUNCTION(BlueprintCallable)
	UDataTable* GetDialogueDataTable();

	UFUNCTION(BlueprintCallable, Category = "Audio")
    UAudioComponent* PlayAudio();

	UPROPERTY()
	class UAudioComponent* BGMusic;

	UPROPERTY()
	TArray<ACommonEnemy*> Enemies;

	UPROPERTY()
	TArray<FVector> EnemyLocations;

	UPROPERTY()
	FVector PlayerLastSavedLocation;

	UFUNCTION()
	void SetPlayerLastLocation(FVector Location);

	UFUNCTION()
	FVector GetPlayerLastLocation();

	UFUNCTION()
	void SaveEnemyLocations(TArray<FVector> AllLocations);

	UFUNCTION()
	void RemoveEnemyAtLocation(FVector Location);

	UFUNCTION()
	void SpawnEnemies();

	UFUNCTION()
	ACommonEnemy* SpawnEnemyAtLocation(FVector Location);
};
