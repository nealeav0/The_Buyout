// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityManager.h"  
#include "Engine/AssetManager.h"

void UAbilityManager::InitializeAbilityDataTables()
{
	// Store all of the possible warrior abilities into the Ability Manager
	TSharedPtr<FStreamableHandle> AssetHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(PlayerAbilityDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			PlayerAbilityDataTable = ReturnedTable;
			if (PlayerAbilityDataTable)
			{
				TArray<FAbilityStruct*> WarriorAbilityData;
				PlayerAbilityDataTable->GetAllRows<FAbilityStruct>(TEXT("Getting warrior abilities"), WarriorAbilityData);
				for (FAbilityStruct* Ability : WarriorAbilityData)
				{
					WarriorAbilities.Add(*Ability);
				}
			}
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Warrior abilities data table was not loaded in.")));
	}

	// Store all of the possible mage abilities into the Ability Manager
	AssetHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(MageAbilityDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			MageAbilityDataTable = ReturnedTable;
			if (MageAbilityDataTable)
			{
				TArray<FAbilityStruct*> MageAbilityData;
				MageAbilityDataTable->GetAllRows<FAbilityStruct>(TEXT("Getting mage abilities"), MageAbilityData);
				for (FAbilityStruct* Ability : MageAbilityData)
				{
					MageAbilities.Add(*Ability);
				}
			}
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Mage abilities data table was not loaded in.")));
	}

	// Store all of the possible ranger abilities into the Ability Manager
	AssetHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(RangerAbilityDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			RangerAbilityDataTable = ReturnedTable;
			if (RangerAbilityDataTable)
			{
				TArray<FAbilityStruct*> RangerAbilityData;
				RangerAbilityDataTable->GetAllRows<FAbilityStruct>(TEXT("Getting ranger abilities"), RangerAbilityData);
				for (FAbilityStruct* Ability : RangerAbilityData)
				{
					RangerAbilities.Add(*Ability);
				}
			}
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Ranger abilities data table was not loaded in.")));
	}
	
	AssetHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(CommonAbilityDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			CommonAbilityDataTable = ReturnedTable;
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Common enemy abilities data table was not loaded in.")));
	}

	AssetHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(EvasiveAbilityDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			EvasiveAbilityDataTable = ReturnedTable;
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Evasive enemy abilities data table was not loaded in.")));
	}

	AssetHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(DefensiveAbilityDataPath);
	if (AssetHandle)
	{
		UDataTable* ReturnedTable = Cast<UDataTable>(AssetHandle->GetLoadedAsset());
		if (ReturnedTable)
		{
			DefensiveAbilityDataTable = ReturnedTable;
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Defensive enemy abilities data table was not loaded in.")));
	}
}

void UAbilityManager::InitializePlayerArray(TArray<FEntityStruct> NewPlayers)
{
	if (!NewPlayers.IsEmpty())
	{
		Players.Empty();
		Players = NewPlayers;
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("The array is empty")));
	}
}

/**
* Return a copy of the Ability Manager's Players array.
*/
TArray<FEntityStruct> UAbilityManager::GetPlayersArray()
{
	return Players;
}

void UAbilityManager::InitializeAbilities(FEntityStruct& Entity)
{
	Entity.Abilities.Empty();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s is here"), *(Entity.EntityName)));
	if (Entity.Name.Equals("warrior"))
	{
		for (FAbilityStruct Ability : WarriorAbilities)
		{
			if (Ability.bIsLearned) 
			{
				Entity.Abilities.Add(Ability);
				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s learned %s"), *(Player.Name), *(Ability.AbilityName)));*/
			}
		}
	}
	else if (Entity.Name.Equals("mage"))
	{
		for (FAbilityStruct Ability : MageAbilities)
		{
			if (Ability.bIsLearned)
			{
				Entity.Abilities.Add(Ability);
				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s learned %s"), *(Player.Name), *(Ability.AbilityName)));*/
			}
		}
	}
	else if (Entity.Name.Equals("ranger"))
	{
		for (FAbilityStruct Ability : RangerAbilities)
		{
			if (Ability.bIsLearned)
			{
				Entity.Abilities.Add(Ability);
			}
		}
	}
	else if (Entity.EnemyType == EEnemyType::COMMON)
	{
		if (CommonAbilityDataTable)
		{
			TArray<FAbilityStruct*> AbilityData;
			CommonAbilityDataTable->GetAllRows<FAbilityStruct>(TEXT("Getting common abilities"), AbilityData);
			for (FAbilityStruct* Ability : AbilityData)
			{
				Entity.Abilities.Add(*Ability);
			}
		}
	}
	else if (Entity.EnemyType == EEnemyType::EVASIVE)
	{
		if (EvasiveAbilityDataTable)
		{
			TArray<FAbilityStruct*> AbilityData;
			EvasiveAbilityDataTable->GetAllRows<FAbilityStruct>(TEXT("Getting evasive abilities"), AbilityData);
			for (FAbilityStruct* Ability : AbilityData)
			{
				Entity.Abilities.Add(*Ability);
			}
		}
	}
	else if (Entity.EnemyType == EEnemyType::DEFENSIVE)
	{
		if (DefensiveAbilityDataTable)
		{
			TArray<FAbilityStruct*> AbilityData;
			DefensiveAbilityDataTable->GetAllRows<FAbilityStruct>(TEXT("Getting defensive abilities"), AbilityData);
			for (FAbilityStruct* Ability : AbilityData)
			{
				Entity.Abilities.Add(*Ability);
			}
		}
	}
}

void UAbilityManager::SelectPlayer(float Navigation)
{
	if (Navigation < 0)
	{
		PlayerIndex--;
		if (PlayerIndex < 0)
			PlayerIndex = Players.Num() - 1;
	}
	else
	{
		PlayerIndex++;
		if (PlayerIndex > Players.Num() - 1)
			PlayerIndex = 0;
	}
	if (GEngine && Players.IsValidIndex(PlayerIndex))
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("Player Index: %d. Player: %s"), PlayerIndex, *(Players[PlayerIndex].Name)));
}

void UAbilityManager::SelectAbility(float Navigation)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("Player Index: %d."), PlayerIndex));
	if (PlayerIndex == 0)
	{
		if (Navigation < 0)
		{
			AbilityIndex--;
			if (AbilityIndex < 0)
				AbilityIndex = WarriorAbilities.Num() - 1;
		}
		else
		{
			AbilityIndex++;
			if (AbilityIndex >= WarriorAbilities.Num())
				AbilityIndex = 0;
		}
		if (GEngine && WarriorAbilities.IsValidIndex(AbilityIndex))
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("Ability Index: %d. Ability: %s"), AbilityIndex, *(WarriorAbilities[AbilityIndex].AbilityName)));
	}
	else if (PlayerIndex == 1)
	{
		if (Navigation < 0)
		{
			AbilityIndex--;
			if (AbilityIndex < 0)
				AbilityIndex = MageAbilities.Num() - 1;
		}
		else
		{
			AbilityIndex++;
			if (AbilityIndex >= MageAbilities.Num())
				AbilityIndex = 0;
		}
		if (GEngine && MageAbilities.IsValidIndex(AbilityIndex))
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("Ability Index: %d. Ability: %s"), AbilityIndex, *(MageAbilities[AbilityIndex].AbilityName)));
	}
	else if (PlayerIndex == 2)
	{
		if (Navigation < 0)
		{
			AbilityIndex--;
			if (AbilityIndex < 0)
				AbilityIndex = RangerAbilities.Num() - 1;
		}
		else
		{
			AbilityIndex++;
			if (AbilityIndex >= RangerAbilities.Num())
				AbilityIndex = 0;
		}
		if (GEngine && RangerAbilities.IsValidIndex(AbilityIndex))
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("Ability Index: %d. Ability: %s"), AbilityIndex, *(RangerAbilities[AbilityIndex].AbilityName)));
	}
}

void UAbilityManager::ConfirmSelection()
{
	if (bSelectingPlayer)
	{
		if (Players.IsValidIndex(PlayerIndex))
		{
			bSelectingPlayer = false;
			bSelectingAbility = true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("We are out of bounds.")));
		}
	}
	else if (bSelectingAbility)
	{
		if (PlayerIndex == 0)
		{
			if (WarriorAbilities.IsValidIndex(AbilityIndex))
			{
				if (!WarriorAbilities[AbilityIndex].bIsLearned)
				{
					LearnAbility(Players[PlayerIndex], WarriorAbilities[AbilityIndex]);
				}
				else
				{
					UpgradeAbility(Players[PlayerIndex], WarriorAbilities[AbilityIndex]);
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("We are out of bounds for warrior ability.")));
			}
		}
		else if (PlayerIndex == 1)
		{
			if (MageAbilities.IsValidIndex(AbilityIndex))
			{
				if (!MageAbilities[AbilityIndex].bIsLearned)
				{
					LearnAbility(Players[PlayerIndex], MageAbilities[AbilityIndex]);
				}
				else
				{
					UpgradeAbility(Players[PlayerIndex], MageAbilities[AbilityIndex]);
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("We are out of bounds for mage ability.")));
			}
		}
		else if (PlayerIndex == 2)
		{
			if (RangerAbilities.IsValidIndex(AbilityIndex))
			{
				if (!RangerAbilities[AbilityIndex].bIsLearned)
				{
					LearnAbility(Players[PlayerIndex], RangerAbilities[AbilityIndex]);
				}
				else
				{
					UpgradeAbility(Players[PlayerIndex], RangerAbilities[AbilityIndex]);
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("We are out of bounds for ranger ability.")));
			}
		}
		PlayerIndex = 0;
		AbilityIndex = 0;
		bSelectingPlayer = true;
		bSelectingAbility = false;
	}
}

/**
* Depending on the state of the Ability Manager, this cancels our Player, Ability, or Target choice.
* If we are currently selecting a player, we set the current PlayerIndex to 0.
* If we are currently selecting an ability, we set the current AbilityIndex to 0 and enable selecting a player.
*/
void UAbilityManager::CancelSelection()
{
	if (bSelectingPlayer)
	{
		PlayerIndex = 0;

		if (!Players.IsValidIndex(PlayerIndex))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Canceling selecting player messed something up.")));
		}
	}
	else if (bSelectingAbility)
	{
		AbilityIndex = 0;

		if (PlayerIndex == 0)
		{
			if (!WarriorAbilities.IsValidIndex(AbilityIndex))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Canceling selecting warrior ability messed something up.")));
			}
		}
		else if (PlayerIndex == 1)
		{
			if (!MageAbilities.IsValidIndex(AbilityIndex))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Canceling selecting mage ability messed something up.")));
			}
		}
		else if (PlayerIndex == 2)
		{
			if (!RangerAbilities.IsValidIndex(AbilityIndex))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Canceling selecting ranger ability messed something up.")));
			}
		}

		bSelectingPlayer = true;
		bSelectingAbility = false;
	}
}

/**
* Learn a new ability from the Ability Manager. If players are trying to learn a new ability, they need the required Ability Points to spend.
*/
void UAbilityManager::LearnAbility(FEntityStruct& Player, FAbilityStruct& Ability)
{
	if (!Ability.bIsLearned)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s is trying to learn %s"), *(Player.Name), *(Ability.AbilityName)));
		if (Player.AbilityPoints >= Ability.APCosts[0])
		{
			Player.AbilityPoints -= Ability.APCosts[0];
			Ability.bIsLearned = true;
			Ability.Level = 1;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s learned %s"), *(Player.Name), *(Ability.AbilityName)));
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s does not have enough Ability Points to learn %s: %d/%d"),
					*(Player.Name), *(Ability.AbilityName), Player.AbilityPoints, Ability.APCosts[0]));
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("%s already knows %s"), *(Player.Name), *(Ability.AbilityName)));
	}
	
}

/**
* Upgrade a known ability from the Ability Manager. If players are trying to upgrade a new ability, they need the required Ability Points to spend.
*/
void UAbilityManager::UpgradeAbility(FEntityStruct& Player, FAbilityStruct& Ability)
{
	if (Ability.bIsLearned)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s is trying to upgrade %s"), *(Player.Name), *(Ability.AbilityName)));
		if (Ability.Level < Ability.MaxLevel)
		{
			if (Ability.APCosts.IsValidIndex(Ability.Level))
			{
				if (Player.AbilityPoints >= Ability.APCosts[Ability.Level])
				{			
					Player.AbilityPoints -= Ability.APCosts[Ability.Level];
					Ability.Level++;
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("Power %.0f -> %.0f"), Ability.Power[Ability.Level - 2], Ability.Power[Ability.Level - 1]));
				}
				else
				{
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s does not have enough Ability Points to upgrade %s: %d/%d"),
							*(Player.Name), *(Ability.AbilityName), Player.AbilityPoints, Ability.APCosts[Ability.Level]));
				}
			}	
			else
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Something is wrong with ability level")));
			}
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s is already at max level"), *(Ability.AbilityName)));
		}
	}
}
