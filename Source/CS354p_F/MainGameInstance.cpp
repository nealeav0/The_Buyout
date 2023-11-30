// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "MainCharacter.h"
#include "CommonEnemy.h"
#include "Engine/AssetManager.h"

UBattleManager* UMainGameInstance::BattleManager()
{
	return IsValid(BattleManagerInstance) ? BattleManagerInstance : BattleManagerInstance = NewObject<UBattleManager>(this, FName("Battle Manager"));
}

void UMainGameInstance::Init()
{
	Super::Init();

	UBattleManager* manager = BattleManager();
	UE_LOG(LogTemp, Warning, TEXT("Data has been updated"));

	// To Do: Intialize the manager for use
	TSharedPtr<FStreamableHandle> AssetHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(PlayerDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			PlayerDataTable = ReturnedTable;
			
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

	AssetHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(CommonDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			CommonDataTable = ReturnedTable;

		}
	}

	AssetHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(CommonBaseDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			CommonBaseDataTable = ReturnedTable;

		}
	}
	
	// some predetermined locations for enemies that we've placed just so we can spawn them in the first place
	EnemyLocations = {	FVector(-455.f, -1440.f, 50.f)	};
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

void UMainGameInstance::SaveEnemyLocations(TArray<FVector> AllLocations)
{
	EnemyLocations = AllLocations;
}

void UMainGameInstance::RemoveEnemyAtLocation(FVector Location)
{
	FString DebugString = "VectorArray: ";

    for (const FVector& Vector : EnemyLocations)
    {
        // Append each FVector's components to the string
        DebugString += FString::Printf(TEXT("(X: %.2f, Y: %.2f, Z: %.2f) "), Vector.X, Vector.Y, Vector.Z);
    }
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugString);

	DebugString = FString::Printf(TEXT("(Location To Remove:: X: %.2f, Y: %.2f, Z: %.2f) "), Location.X, Location.Y, Location.Z);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugString);

	EnemyLocations.Remove(Location);

	DebugString = "VectorArray: ";

    for (const FVector& Vector : EnemyLocations)
    {
        // Append each FVector's components to the string
        DebugString += FString::Printf(TEXT("(X: %.2f, Y: %.2f, Z: %.2f) "), Vector.X, Vector.Y, Vector.Z);
    }
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugString);
}

void UMainGameInstance::SpawnEnemies()
{
	for (FVector Location : EnemyLocations) {
		SpawnEnemyAtLocation(Location);
	}
}

void UMainGameInstance::SpawnEnemyAtLocation(FVector Location)
{
	if (GetWorld())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);

        ACommonEnemy* NewActor = GetWorld()->SpawnActor<ACommonEnemy>(ACommonEnemy::StaticClass(), Location, SpawnRotation, SpawnParams);
    }
}
