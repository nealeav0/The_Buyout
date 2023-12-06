// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "MainCharacter.h"
#include "CommonEnemy.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"

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

	AssetHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(DialogueDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			DialogueDataTable = ReturnedTable;

		}
	}

	// some predetermined locations in the overworld for enemies that we've placed just so we can spawn them in the first place
	// EnemyLocations = {	FVector(-455.f, -1440.f, 50.f)	}; // old overworld
	EnemyLocations = {
		FVector(930.f, 20.f, 56.f),
		FVector(600.f, 1160.f, 56.f),
		FVector(500.f, -1190.f, 56.f)
	};

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
	EnemyLocations.Remove(Location);
}

void UMainGameInstance::SpawnEnemies()
{
	for (FVector Location : EnemyLocations) {
		SpawnEnemyAtLocation(Location);
	}
}

ACommonEnemy* UMainGameInstance::SpawnEnemyAtLocation(FVector Location)
{
	// can't put this in init; otherwise will crash, so we'll just load it in only the first time this is called 
	if (!CommonEnemyBPClass) {
		static ConstructorHelpers::FClassFinder<ACommonEnemy> EnemyBPClass(TEXT("/Game/Blueprints/BP_CommonEnemy"));
		if (EnemyBPClass.Class)
			CommonEnemyBPClass = EnemyBPClass.Class;
	}

	if (GetWorld())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);

        return GetWorld()->SpawnActor<ACommonEnemy>(CommonEnemyBPClass, Location, SpawnRotation, SpawnParams);
    }
	return nullptr;
}
