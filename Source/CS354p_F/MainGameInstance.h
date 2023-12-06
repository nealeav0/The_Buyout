// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BattleManager.h"
#include "AbilityManager.h"
#include "EnemyBase.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMainGameInstance();

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

	UPROPERTY(EditAnywhere)
	UDataTable* EndingSceneDialogue;

	// /Script/Engine.DataTable'/Game/Data/Player_Base_Stats.Player_Base_Stats'
	UPROPERTY()
	FSoftObjectPath PlayerBaseDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Player_Base_Stats.Player_Base_Stats'"));

	// /Script/Engine.DataTable'/Game/Data/Enemy_Base_Stats.Enemy_Base_Stats'
	UPROPERTY()
	FSoftObjectPath EnemyBaseDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/Enemy_Base_Stats.Enemy_Base_Stats'"));

	// /Script/Engine.DataTable'/Game/Data/NewDialogue.NewDialogue'
	UPROPERTY(VisibleAnywhere)
	FSoftObjectPath DialogueDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/NewDialogue.NewDialogue'"));

	// /Script/Engine.DataTable'/Game/Data/EndingSceneDialogue.EndingSceneDialogue'
	UPROPERTY(VisibleAnywhere)
	FSoftObjectPath EndingSceneDialogueDataPath = FSoftObjectPath(TEXT("DataTable'/Game/Data/EndingSceneDialogue.EndingSceneDialogue'"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACommonEnemy> CommonEnemyBPClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEvasiveEnemy> EvasiveEnemyBPClass;

	virtual void Init() override;

	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable) 
	UBattleManager* BattleManager();

	UFUNCTION(BlueprintCallable)
	UAbilityManager* AbilityManager();

	void UpdateBattleManager();

	UFUNCTION(BlueprintCallable)
	UDataTable* GetDialogueDataTable();

	UFUNCTION(BlueprintCallable)
	UDataTable* GetEndingSceneDialogue();

	UFUNCTION(BlueprintCallable, Category = "Audio")
    UAudioComponent* PlayBGAudio();

	UFUNCTION(BlueprintCallable, Category = "Audio")
    UAudioComponent* PlayBattleAudio();

	UFUNCTION(BlueprintCallable, Category = "Audio")
    void ToggleMute();

	UPROPERTY()
	class UAudioComponent* BGMusic;

	UPROPERTY()
	class UAudioComponent* BattleMusic;

	UPROPERTY()
	float Volume;

	UPROPERTY()
	TArray<AEnemyBase*> Enemies;

	UPROPERTY()
	TArray<EEnemyType> EnemyTypes;

	UFUNCTION()
	void SaveEnemyTypes(TArray<EEnemyType> AllEnemyTypes);

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
	void RemoveEnemy(FEntityStruct Enemy);

    UFUNCTION()
	void SpawnEnemies();

	UFUNCTION()
	AEnemyBase* SpawnEnemyAtLocation(EEnemyType EnemyType, FVector Location);

	UPROPERTY(BlueprintReadWrite)
	bool begDialogueDone = false;

	UPROPERTY(BlueprintReadWrite)
	bool endDialogueDone = false;
};
