// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "MainCharacter.h"
#include "CommonEnemy.h"
#include "EntityBase.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"

UMainGameInstance::UMainGameInstance() {
	static ConstructorHelpers::FClassFinder<ACommonEnemy> CommonBPClass(TEXT("/Game/Blueprints/BP_CommonEnemy"));
	if (CommonBPClass.Class) {
		CommonEnemyBPClass = CommonBPClass.Class; 
	}

	static ConstructorHelpers::FClassFinder<AEvasiveEnemy> EvasiveBPClass(TEXT("/Game/Blueprints/BP_EvasiveEnemy"));
	if (EvasiveBPClass.Class) {
		EvasiveEnemyBPClass = EvasiveBPClass.Class;
	}
}

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

	BattleManagerInstance = BattleManager();
	AbilityManagerInstance = AbilityManager();

	AbilityManagerInstance->InitializeAbilityDataTables();

	TSharedPtr<FStreamableHandle> AssetHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(PlayerBaseDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			PlayerBaseDataTable = ReturnedTable;

		}
	}

	AssetHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(EnemyBaseDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			EnemyBaseDataTable = ReturnedTable;

		}
	}

	AssetHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(MapEnemiesDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			MapEnemiesDataTable = ReturnedTable;

		}
	}

	AssetHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(DialogueDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			DialogueDataTable = ReturnedTable;

		}
	}

	AssetHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(EndingSceneDialogueDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			EndingSceneDialogue = ReturnedTable;

		}
	}

	// some predetermined locations in the overworld for enemies that we've placed just so we can spawn them in the first place
	// EnemyLocations = {	FVector(-455.f, -1440.f, 50.f)	}; // old overworld

	if (MapEnemiesDataTable) 
	{
		/*if (PlayerAbilityDataTable)
		{
			TArray<FAbilityStruct*> WarriorAbilityData;
			PlayerAbilityDataTable->GetAllRows<FAbilityStruct>(TEXT("Getting warrior abilities"), WarriorAbilityData);
			for (FAbilityStruct* Ability : WarriorAbilityData)
			{
				WarriorAbilities.Add(*Ability);
			}
		}*/
		Enemies.Empty();
		TArray<FEntityStruct*> MapEnemiesData;
		MapEnemiesDataTable->GetAllRows<FEntityStruct>(TEXT("Getting Map Enemies"), MapEnemiesData);
		for (FEntityStruct* MapEnemy : MapEnemiesData)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Magenta, FString::Printf(TEXT("%s is here. Lv. %d"), *(MapEnemy->EntityName), MapEnemy->Level));
			}
			Enemies.Add(*MapEnemy);
			EnemyLocations.Add(MapEnemy->Location);
		}
	}

	/*EnemyLocations = {
		FVector(1500.f, 20.f, 0.f),
		FVector(1000.f, 1160.f, 0.f),
		FVector(900.f, -1150.f, 0.f)
	};*/

	Volume = 0.75f;
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

UDataTable* UMainGameInstance::GetDialogueDataTable()
{
	return DialogueDataTable;
}

UDataTable* UMainGameInstance::GetEndingSceneDialogue()
{
	return EndingSceneDialogue;
}

UAudioComponent* UMainGameInstance::PlayBGAudio()
{
	USoundBase* SoundCue = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("/Game/Assets/BGMusicCue")));

	// const UObject * WorldContextObject, USoundBase * Sound, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundConcurrency * ConcurrencySettings, bool bPersistAcrossLevelTransition, bool bAutoDestroy
	BGMusic = UGameplayStatics::CreateSound2D(GetWorld(), SoundCue, Volume, 1.0f, 0.0f, nullptr, true, true);
	BGMusic->Play();
	return BGMusic;
}

UAudioComponent *UMainGameInstance::PlayBattleAudio()
{
	USoundBase* SoundCue = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("/Game/Assets/BattleMusicCue")));

	BattleMusic = UGameplayStatics::CreateSound2D(GetWorld(), SoundCue, Volume, 1.0f, 0.0f, nullptr, false, true);
	BattleMusic->Play();
	return BattleMusic;
}

void UMainGameInstance::ToggleMute()
{
	Volume = (Volume == 0.0f) ? 0.75f : 0.0f;

	if (BGMusic)
		BGMusic->SetVolumeMultiplier(Volume);

	if (BattleMusic)
		BattleMusic->SetVolumeMultiplier(Volume);

}

void UMainGameInstance::SaveEnemyTypes(TArray<EEnemyType> AllEnemyTypes)
{
	EnemyTypes = AllEnemyTypes;
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

void UMainGameInstance::RemoveEnemy(FEntityStruct Enemy)
{
	int32 index = EnemyLocations.Find(Enemy.Location);
	EnemyLocations.RemoveAt(index);
	Enemies.RemoveAt(index);
}

void UMainGameInstance::SpawnEnemies()
{
	for (int i = 0; i < EnemyLocations.Num(); i++) {
		SpawnEnemyAtLocation(Enemies[i], EnemyLocations[i]);
	}
}

/**
* Check what allies this enemy may have an add it to the Enemies array of the reference
*/
AEnemyBase* UMainGameInstance::SpawnEnemyAtLocation(FEntityStruct Enemy, FVector Location)
{
	AEnemyBase* Result = nullptr;
	if (GetWorld())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
		
		if (Enemy.EnemyType == EEnemyType::COMMON)
		{
			Result = GetWorld()->SpawnActor<ACommonEnemy>(CommonEnemyBPClass, Location, SpawnRotation, SpawnParams);
			Result->Enemies[0].Level = Enemy.Level;
		}
		else if (Enemy.EnemyType == EEnemyType::EVASIVE)
		{
			Result = GetWorld()->SpawnActor<AEvasiveEnemy>(EvasiveEnemyBPClass, Location, SpawnRotation, SpawnParams);
			Result->Enemies[0].Level = Enemy.Level;
		}

		if (!Enemy.Allies.IsEmpty())
		{
			for (FEntityAllyStruct Ally : Enemy.Allies)
			{
				Result->Enemies.Add(FEntityStruct(Ally.EntityName, Ally.EnemyType, Ally.Level));
			}
		}
		return Result;
    }

	return Result;

}
