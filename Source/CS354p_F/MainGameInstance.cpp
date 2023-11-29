// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "MainCharacter.h"
#include "CommonEnemy.h"
#include "Engine/AssetManager.h"

UBattleManager* UMainGameInstance::BattleManager()
{
	return IsValid(BattleManagerInstance) ? BattleManagerInstance : BattleManagerInstance = NewObject<UBattleManager>(this, FName("Battle Manager"));
}

UAbilityManager* UMainGameInstance::AbilityManager()
{
	return IsValid(AbilityManagerInstance) ? AbilityManagerInstance : AbilityManagerInstance = NewObject<UAbilityManager>(this, FName("Ability Manager"));
}

void UMainGameInstance::Init()
{
	Super::Init();

	UBattleManager* BManager = BattleManager();
	UAbilityManager* AManager = AbilityManager();

	// To Do: Intialize the manager for use
	TSharedPtr<FStreamableHandle> AssetHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(PlayerAbilityDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			PlayerAbilityDataTable = ReturnedTable;
			
		}
	}

	AssetHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(PlayerBaseDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			PlayerBaseDataTable = ReturnedTable;

		}
	}

	AssetHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(EnemyAbilityDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			EnemyAbilityDataTable = ReturnedTable;

		}
	}

	AssetHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(EnemyBaseDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			EnemyBaseDataTable = ReturnedTable;

		}
	}
}

void UMainGameInstance::Shutdown()
{
	Super::Shutdown();
	if (IsValid(BattleManagerInstance))
	{
		// To Do: Clear out allocated memories. Use delete not free if you are removing an instance.
	}
}

void UMainGameInstance::UpdateBattleManager()
{
	UBattleManager* manager = BattleManager();
}

void UMainGameInstance::SetPlayerLastLocation(FVector Location)
{
	PlayerLastSavedLocation = Location;
}

FVector UMainGameInstance::GetPlayerLastLocation()
{
    return PlayerLastSavedLocation;
}
