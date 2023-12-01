// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityManager.h"
#include "Engine/AssetManager.h"

void UAbilityManager::InitializeAbilityDataTables()
{
	// Store the all the possible warrior abilities into the Ability Manager
	TSharedPtr<FStreamableHandle> AssetHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(PlayerAbilityDataPath);
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

	// Store the all the possible mage abilities into the Ability Manager
	AssetHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(MageAbilityDataPath);
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
}

void UAbilityManager::InitializePlayerArray(TArray<FEntityStruct>& NewPlayers)
{
	if (!NewPlayers.IsEmpty())
	{
		Players.Empty();
		Players = NewPlayers;
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("The array is not empty")));
	}
}

/**
* Return a copy of the Ability Manager's Players array.
*/
TArray<FEntityStruct> UAbilityManager::GetPlayersArray()
{
	return Players;
}

void UAbilityManager::InitializeAbilities(FEntityStruct& Player)
{
	Player.Abilities.Empty();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s is here"), *(Player.Name)));
	if (Player.Name.Equals("warrior"))
	{
		for (FAbilityStruct Ability : WarriorAbilities)
		{
			if (Ability.bIsLearned) 
			{
				Player.Abilities.Add(Ability);
				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s learned %s"), *(Player.Name), *(Ability.AbilityName)));*/
			}
		}
	}
	else if (Player.Name.Equals("mage"))
	{
		for (FAbilityStruct Ability : MageAbilities)
		{
			if (Ability.bIsLearned)
			{
				Player.Abilities.Add(Ability);
				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s learned %s"), *(Player.Name), *(Ability.AbilityName)));*/
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
		PlayerIndex = 0;
		AbilityIndex = 0;
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
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("Power %.0f -> %.0f"), Ability.Power[Ability.Level - 1], Ability.Power[Ability.Level]));
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
