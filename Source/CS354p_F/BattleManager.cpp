// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManager.h"
#include "MainPlayerController.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameInstance.h"

AMainPlayerController* MainPlayerController;
bool bEnemyDied;

UBattleManager::UBattleManager()
{

	MainPlayerController = nullptr;

	GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	// /Script/Engine.Blueprint'/Game/Blueprints/BP_CommonEnemy.BP_CommonEnemy'
	static ConstructorHelpers::FClassFinder<ACommonEnemy> CommonBPClass(TEXT("/Game/Blueprints/BP_CommonEnemy"));
	if (CommonBPClass.Class)
		CommonEnemyBP = CommonBPClass.Class;

	// /Script/Engine.Blueprint'/Game/Blueprints/BP_EvasiveEnemy.BP_EvasiveEnemy'
	static ConstructorHelpers::FClassFinder<AEvasiveEnemy> EvasiveBPClass(TEXT("/Game/Blueprints/BP_EvasiveEnemy"));
	if (EvasiveBPClass.Class)
		EvasiveEnemyBP = EvasiveBPClass.Class;

	static ConstructorHelpers::FClassFinder<ADefensiveEnemy> DefensiveBPClass(TEXT("/Game/Blueprints/BP_DefensiveEnemy"));
	if (DefensiveBPClass.Class) {
		DefensiveEnemyBP = DefensiveBPClass.Class;
	}

	// /Script/Engine.Blueprint'/Game/Blueprints/BP_SupportEnemy.BP_SupportEnemy'
	static ConstructorHelpers::FClassFinder<ASupportEnemy> SupportBPClass(TEXT("/Game/Blueprints/BP_SupportEnemy"));
	if (SupportBPClass.Class) {
		SupportEnemyBP = SupportBPClass.Class;
	}

	// /Script/Engine.Blueprint'/Game/Blueprints/BP_SeniorEnemy.BP_SeniorEnemy'
	static ConstructorHelpers::FClassFinder<ASeniorEnemy> SeniorBPClass(TEXT("/Game/Blueprints/BP_SeniorEnemy"));
	if (SeniorBPClass.Class) {
		SeniorEnemyBP = SeniorBPClass.Class;
	}

	// /Script/Engine.Blueprint'/Game/Blueprints/BP_DonorEnemy.BP_DonorEnemy'
	static ConstructorHelpers::FClassFinder<ADonorEnemy> DonorBPClass(TEXT("/Game/Blueprints/BP_DonorEnemy"));
	if (DonorBPClass.Class) {
		DonorEnemyBP = DonorBPClass.Class;
	}

	// /Script/Engine.Blueprint'/Game/Blueprints/BP_BaronEnemy.BP_BaronEnemy'
	static ConstructorHelpers::FClassFinder<ABaronEnemy> BaronBPClass(TEXT("/Game/Blueprints/BP_BaronEnemy"));
	if (BaronBPClass.Class) {
		BaronEnemyBP = BaronBPClass.Class;
	}
}

/**
* Using the keyboard, you can navigate a carousel to choose a party member.
*/
void UBattleManager::SelectPlayer(int32 index)
{
	PlayerIndex = index;

	// switch mesh material to indicate which player we're using
	MainPlayerController->SwitchMeshMaterial(index);

	if (GEngine && Players.IsValidIndex(PlayerIndex))
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("Player Index: %d. Player: %s"), PlayerIndex, *(Players[PlayerIndex].Name)));
}

/**
* Using the keyboard, you can navigate a carousel to choose a ability.
*/
void UBattleManager::SelectAbility(int32 index)
{
	AbilityIndex = index;
	// Remember to dereference FStrings to get the wchar_t inside.
	if (GEngine && Players[PlayerIndex].Abilities.IsValidIndex(AbilityIndex))
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("Ability Index: %d. Ability: %s"), AbilityIndex, *(Players[PlayerIndex].Abilities[AbilityIndex].AbilityName)));
}

/**
* Using the keyboard, you can navigate a carousel to choose a target. If the ability targets an ally, then you must choose one or all of the party members 
*/
void UBattleManager::SelectTarget(int32 index)
{		
	TargetIndex = index;

	if (Players[PlayerIndex].Abilities[AbilityIndex].TargetType == ETargetTypeEnum::ALLY || Players[PlayerIndex].Abilities[AbilityIndex].TargetType == ETargetTypeEnum::ALLIES)
	{
		if (GEngine && Players.IsValidIndex(TargetIndex))
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("Target Index: %d. Player: %s"), TargetIndex, *(Players[TargetIndex].Name)));
	} 
	else
	{
		if (GEngine && Enemies.IsValidIndex(TargetIndex))
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("Target Index: %d. Enemy: %s"), TargetIndex, *(Enemies[TargetIndex].Name)));
	}	
}

/**
* Depending on the state of the Battle Manager, this determines the validity our Player, Ability, and Target choice.
* If we are currently selecting a player, we cannot choose a player that is dead or lacks actions.
* If we are currently selecting an ability, we cannot choose an ability that is under cooldown.
* If we are currently selecting a target, we cannot choose a dead enemy.
*/
void UBattleManager::ConfirmSelection()
{
	HandlePlayerInput(Players[PlayerIndex].Abilities[AbilityIndex]);
}

/**
* Depending on the state of the Battle Manager, this cancels our Player, Ability, or Target choice.
* If we are currently selecting a player, we set the current PlayerIndex to 0.
* If we are currently selecting an ability, we set the current AbilityIndex to 0 and enable selecting a player.
* If we are currently selecting a target, we set the current TargetIndex to 0 and enable selecting an ability.
*/
void UBattleManager::CancelSelection()
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

		if (!Players[PlayerIndex].Abilities.IsValidIndex(AbilityIndex))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Canceling selecting ability messed something up.")));
		}

		bSelectingPlayer = true;
		bSelectingAbility = false;
	}
	else if (bSelectingTarget)
	{
		TargetIndex = 0;

		if (Players[PlayerIndex].Abilities[AbilityIndex].TargetType == ETargetTypeEnum::ALLY || Players[PlayerIndex].Abilities[AbilityIndex].TargetType == ETargetTypeEnum::ALLIES)
		{
			if (!Players.IsValidIndex(TargetIndex))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Canceling selecting target player messed something up.")));
			}
		}
		else
		{
			if (!Enemies.IsValidIndex(TargetIndex))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Canceling selecting target enemy messed something up.")));
			}
		}

		bSelectingAbility = true;
		bSelectingTarget = false;
	}
}

void UBattleManager::DefendHandler()
{
	if (PlayerActions[PlayerIndex] == 0)
	{
		for (int i = PlayerActions.Num() - 1; i >= 0; i--)
		{
			if (PlayerActions[i] > 0)
			{
				PlayerIndex = i;
			}
		}
	}
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("%s is defending."), *(Players[PlayerIndex].Name)));
	Players[PlayerIndex].bIsDefending = true;
	PlayerActions[PlayerIndex]--;
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::PlayerTurn, 0.5f, false);
}

void UBattleManager::EscapeHandler()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("The Player ran away.")));
	bBattleEnd = true;
	Cast<AMainCharacter>(MainPlayerController->GetPawn())->Escape();
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::LeaveBattle, 0.5f, false);
}

/**
* Prepare to segue out of battle. At the end of battle, players are given EXP. In addition, all temporary buffs, debuffs, cooldowns, and status effects are reset back to 0
*/
void UBattleManager::LeaveBattle() {
	// handle removing enemies in overworld iff ALL enemies in this battle have been killed
	if (CheckEnemiesIsDead())
		GameInstance->RemoveEnemy(Enemies[0]);
	// before we leave the battle, let's clean up a bit
	
	// We should tally up our EXP at the end of battle
	HandleEXP();

	// We should set all of the temporary stuff back to 0;
	for (FEntityStruct& Player : Players)
	{
		if (Player.bIsDead)
		{
			Player.bIsDead = false;
			Player.Health = 1;
		}
		Player.AttackBuff = 0;
		Player.MagicAttackBuff = 0;
		Player.DefenseBuff = 0;
		Player.MagicDefenseBuff = 0;
		Player.AccuracyBuff = 0;
		Player.EvasionBuff = 0;
		Player.BurnStacks = 0;
		Player.ChillStacks = 0;
		Player.StunStacks = 0;
		Player.PoisonStacks = 0;

		for (FAbilityStruct& Ability : Player.Abilities)
		{
			Ability.Cooldown = 0;
		}
	}

	MainPlayerController->CloseBattleUI();
	MainPlayerController = nullptr;
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("AggieMap")), true, "");
	
}

void UBattleManager::InitializeEnemyStats(FEntityStruct& Enemy)
{
	if (GameInstance->EnemyBaseDataTable)
	{
		FEntityStruct* EnemyBase;
		switch (Enemy.EnemyType)
		{
		case EEnemyType::COMMON:
			EnemyBase = GameInstance->EnemyBaseDataTable->FindRow<FEntityStruct>(FName(TEXT("common")), FString(TEXT("Getting Common Stats")));

			if (EnemyBase)
			{
				if (Enemy.Level == 0)
				{
					Enemy.Level = 1;
				}
				Enemy.Name = (*EnemyBase).Name;
				Enemy.EntityName = (*EnemyBase).EntityName;
				Enemy.EntityType = (*EnemyBase).EntityType;
				Enemy.MaxHealth = FMath::Floor((*EnemyBase).MaxHealth * FMath::Pow(1.191, Enemy.Level));
				Enemy.Health = Enemy.MaxHealth;
				Enemy.Attack = FMath::Floor((*EnemyBase).Attack * FMath::Pow(1.29, Enemy.Level));
				Enemy.MagicAttack = FMath::Floor((*EnemyBase).MagicAttack * FMath::Pow(1.29, Enemy.Level));
				Enemy.Defense = FMath::Floor((*EnemyBase).Defense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.MagicDefense = FMath::Floor((*EnemyBase).MagicDefense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.Accuracy = FMath::Floor((*EnemyBase).Accuracy * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.Evasion = FMath::Floor((*EnemyBase).Evasion * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.EXP = FMath::Floor((*EnemyBase).EXP * FMath::Pow(1.2, Enemy.Level));
				Enemy.AbilityPoints = FMath::Floor((*EnemyBase).AbilityPoints * FMath::Pow(1.25, Enemy.Level));
				Enemy.ElementalResistances = (*EnemyBase).ElementalResistances;
			}
			break;
		case EEnemyType::EVASIVE:
			EnemyBase = GameInstance->EnemyBaseDataTable->FindRow<FEntityStruct>(FName(TEXT("evasive")), FString(TEXT("Getting evasive Stats")));

			if (EnemyBase)
			{
				if (Enemy.Level == 0)
				{
					Enemy.Level = 1;
				}
				Enemy.Name = (*EnemyBase).Name;
				Enemy.EntityName = (*EnemyBase).EntityName;
				Enemy.EntityType = (*EnemyBase).EntityType;
				Enemy.MaxHealth = FMath::Floor((*EnemyBase).MaxHealth * FMath::Pow(1.191, Enemy.Level));
				Enemy.Health = Enemy.MaxHealth;
				Enemy.Attack = FMath::Floor((*EnemyBase).Attack * FMath::Pow(1.29, Enemy.Level));
				Enemy.MagicAttack = FMath::Floor((*EnemyBase).MagicAttack * FMath::Pow(1.29, Enemy.Level));
				Enemy.Defense = FMath::Floor((*EnemyBase).Defense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.MagicDefense = FMath::Floor((*EnemyBase).MagicDefense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.Accuracy = FMath::Floor((*EnemyBase).Accuracy * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.Evasion = FMath::Floor((*EnemyBase).Evasion * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.EXP = FMath::Floor((*EnemyBase).EXP * FMath::Pow(1.2, Enemy.Level));
				Enemy.AbilityPoints = FMath::Floor((*EnemyBase).AbilityPoints * FMath::Pow(1.25, Enemy.Level));
				Enemy.ElementalResistances = (*EnemyBase).ElementalResistances;
			}
			break;
		case EEnemyType::DEFENSIVE:
			EnemyBase = GameInstance->EnemyBaseDataTable->FindRow<FEntityStruct>(FName(TEXT("defensive")), FString(TEXT("Getting defensive Stats")));

			if (EnemyBase)
			{
				if (Enemy.Level == 0)
				{
					Enemy.Level = 1;
				}
				Enemy.Name = (*EnemyBase).Name;
				Enemy.EntityName = (*EnemyBase).EntityName;
				Enemy.EntityType = (*EnemyBase).EntityType;
				Enemy.MaxHealth = FMath::Floor((*EnemyBase).MaxHealth * FMath::Pow(1.191, Enemy.Level));
				Enemy.Health = Enemy.MaxHealth;
				Enemy.Attack = FMath::Floor((*EnemyBase).Attack * FMath::Pow(1.29, Enemy.Level));
				Enemy.MagicAttack = FMath::Floor((*EnemyBase).MagicAttack * FMath::Pow(1.29, Enemy.Level));
				Enemy.Defense = FMath::Floor((*EnemyBase).Defense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.MagicDefense = FMath::Floor((*EnemyBase).MagicDefense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.Accuracy = FMath::Floor((*EnemyBase).Accuracy * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.Evasion = FMath::Floor((*EnemyBase).Evasion * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.EXP = FMath::Floor((*EnemyBase).EXP * FMath::Pow(1.2, Enemy.Level));
				Enemy.AbilityPoints = FMath::Floor((*EnemyBase).AbilityPoints * FMath::Pow(1.25, Enemy.Level));
				Enemy.ElementalResistances = (*EnemyBase).ElementalResistances;
			}
			break;
		case EEnemyType::SUPPORT:
			EnemyBase = GameInstance->EnemyBaseDataTable->FindRow<FEntityStruct>(FName(TEXT("support")), FString(TEXT("Getting support Stats")));

			if (EnemyBase)
			{
				if (Enemy.Level == 0)
				{
					Enemy.Level = 1;
				}
				Enemy.Name = (*EnemyBase).Name;
				Enemy.EntityName = (*EnemyBase).EntityName;
				Enemy.EntityType = (*EnemyBase).EntityType;
				Enemy.MaxHealth = FMath::Floor((*EnemyBase).MaxHealth * FMath::Pow(1.191, Enemy.Level));
				Enemy.Health = Enemy.MaxHealth;
				Enemy.Attack = FMath::Floor((*EnemyBase).Attack * FMath::Pow(1.29, Enemy.Level));
				Enemy.MagicAttack = FMath::Floor((*EnemyBase).MagicAttack * FMath::Pow(1.29, Enemy.Level));
				Enemy.Defense = FMath::Floor((*EnemyBase).Defense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.MagicDefense = FMath::Floor((*EnemyBase).MagicDefense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.Accuracy = FMath::Floor((*EnemyBase).Accuracy * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.Evasion = FMath::Floor((*EnemyBase).Evasion * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.EXP = FMath::Floor((*EnemyBase).EXP * FMath::Pow(1.2, Enemy.Level));
				Enemy.AbilityPoints = FMath::Floor((*EnemyBase).AbilityPoints * FMath::Pow(1.25, Enemy.Level));
				Enemy.ElementalResistances = (*EnemyBase).ElementalResistances;
			}
			break;
		case EEnemyType::SENIOR:
			EnemyBase = GameInstance->EnemyBaseDataTable->FindRow<FEntityStruct>(FName(TEXT("senior")), FString(TEXT("Getting senior Stats")));

			if (EnemyBase)
			{
				if (Enemy.Level == 0)
				{
					Enemy.Level = 1;
				}
				Enemy.Name = (*EnemyBase).Name;
				Enemy.EntityName = (*EnemyBase).EntityName;
				Enemy.EntityType = (*EnemyBase).EntityType;
				Enemy.MaxHealth = FMath::Floor((*EnemyBase).MaxHealth * FMath::Pow(1.191, Enemy.Level));
				Enemy.Health = Enemy.MaxHealth;
				Enemy.Attack = FMath::Floor((*EnemyBase).Attack * FMath::Pow(1.29, Enemy.Level));
				Enemy.MagicAttack = FMath::Floor((*EnemyBase).MagicAttack * FMath::Pow(1.29, Enemy.Level));
				Enemy.Defense = FMath::Floor((*EnemyBase).Defense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.MagicDefense = FMath::Floor((*EnemyBase).MagicDefense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.Accuracy = FMath::Floor((*EnemyBase).Accuracy * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.Evasion = FMath::Floor((*EnemyBase).Evasion * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.EXP = FMath::Floor((*EnemyBase).EXP * FMath::Pow(1.2, Enemy.Level));
				Enemy.AbilityPoints = FMath::Floor((*EnemyBase).AbilityPoints * FMath::Pow(1.25, Enemy.Level));
				Enemy.ElementalResistances = (*EnemyBase).ElementalResistances;
			}
			break;
		case EEnemyType::DONOR:
			EnemyBase = GameInstance->EnemyBaseDataTable->FindRow<FEntityStruct>(FName(TEXT("donor")), FString(TEXT("Getting donor Stats")));

			if (EnemyBase)
			{
				if (Enemy.Level == 0)
				{
					Enemy.Level = 1;
				}
				Enemy.Name = (*EnemyBase).Name;
				Enemy.EntityName = (*EnemyBase).EntityName;
				Enemy.EntityType = (*EnemyBase).EntityType;
				Enemy.MaxHealth = FMath::Floor((*EnemyBase).MaxHealth * FMath::Pow(1.191, Enemy.Level));
				Enemy.Health = Enemy.MaxHealth;
				Enemy.Attack = FMath::Floor((*EnemyBase).Attack * FMath::Pow(1.29, Enemy.Level));
				Enemy.MagicAttack = FMath::Floor((*EnemyBase).MagicAttack * FMath::Pow(1.29, Enemy.Level));
				Enemy.Defense = FMath::Floor((*EnemyBase).Defense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.MagicDefense = FMath::Floor((*EnemyBase).MagicDefense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.Accuracy = FMath::Floor((*EnemyBase).Accuracy * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.Evasion = FMath::Floor((*EnemyBase).Evasion * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.EXP = FMath::Floor((*EnemyBase).EXP * FMath::Pow(1.2, Enemy.Level));
				Enemy.AbilityPoints = FMath::Floor((*EnemyBase).AbilityPoints * FMath::Pow(1.25, Enemy.Level));
				Enemy.ElementalResistances = (*EnemyBase).ElementalResistances;
			}
			break;
		case EEnemyType::BARON:
			EnemyBase = GameInstance->EnemyBaseDataTable->FindRow<FEntityStruct>(FName(TEXT("baron")), FString(TEXT("Getting baron Stats")));

			if (EnemyBase)
			{
				if (Enemy.Level == 0)
				{
					Enemy.Level = 1;
				}
				Enemy.Name = (*EnemyBase).Name;
				Enemy.EntityName = (*EnemyBase).EntityName;
				Enemy.EntityType = (*EnemyBase).EntityType;
				Enemy.MaxHealth = FMath::Floor((*EnemyBase).MaxHealth * FMath::Pow(1.191, Enemy.Level));
				Enemy.Health = Enemy.MaxHealth;
				Enemy.Attack = FMath::Floor((*EnemyBase).Attack * FMath::Pow(1.29, Enemy.Level));
				Enemy.MagicAttack = FMath::Floor((*EnemyBase).MagicAttack * FMath::Pow(1.29, Enemy.Level));
				Enemy.Defense = FMath::Floor((*EnemyBase).Defense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.MagicDefense = FMath::Floor((*EnemyBase).MagicDefense + (*EnemyBase).Defense * (Enemy.Level / 25));
				Enemy.Accuracy = FMath::Floor((*EnemyBase).Accuracy * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.Evasion = FMath::Floor((*EnemyBase).Evasion * 0.84 * FMath::Pow(1.0844, Enemy.Level) * 2);
				Enemy.EXP = FMath::Floor((*EnemyBase).EXP * FMath::Pow(1.2, Enemy.Level));
				Enemy.AbilityPoints = FMath::Floor((*EnemyBase).AbilityPoints * FMath::Pow(1.25, Enemy.Level));
				Enemy.ElementalResistances = (*EnemyBase).ElementalResistances;
			}
			break;
		}
	}
}

void UBattleManager::SpawnEnemies()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	for (int i = 0; i < Enemies.Num(); i++)
	{
		AEnemyBase* Enemy = nullptr;
		switch (Enemies[i].EnemyType)
		{
		case EEnemyType::COMMON:
			if (!CommonEnemyBP)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("CommonEnemyBP does not exist.")));
			}
			Enemy = GetWorld()->SpawnActor<ACommonEnemy>(CommonEnemyBP, EnemyPositions[i], FRotator(), SpawnParams);
			break;
		case EEnemyType::EVASIVE:
			if (!EvasiveEnemyBP)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("EvasiveEnemyBP does not exist.")));
			}
			Enemy = GetWorld()->SpawnActor<AEvasiveEnemy>(EvasiveEnemyBP, EnemyPositions[i], FRotator(), SpawnParams);
			break;
		case EEnemyType::DEFENSIVE:
			if (!DefensiveEnemyBP)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("DefensiveEnemyBP does not exist.")));
			}
			Enemy = GetWorld()->SpawnActor<ADefensiveEnemy>(DefensiveEnemyBP, EnemyPositions[i], FRotator(), SpawnParams);
			break;
		case EEnemyType::SUPPORT:
			if (!SupportEnemyBP)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("SupportEnemyBP does not exist.")));
			}
			Enemy = GetWorld()->SpawnActor<ASupportEnemy>(SupportEnemyBP, EnemyPositions[i], FRotator(), SpawnParams);
			break;
		case EEnemyType::SENIOR:
			if (!SeniorEnemyBP)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("SeniorEnemyBP does not exist.")));
			}
			Enemy = GetWorld()->SpawnActor<ASeniorEnemy>(SeniorEnemyBP, EnemyPositions[i], FRotator(), SpawnParams);
			break;
		case EEnemyType::DONOR:
			if (!DonorEnemyBP)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("DonorEnemyBP does not exist.")));
			}
			Enemy = GetWorld()->SpawnActor<ADonorEnemy>(DonorEnemyBP, EnemyPositions[i], FRotator(), SpawnParams);
			break;
		case EEnemyType::BARON:
			if (!BaronEnemyBP)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("BarronEnemyBP does not exist.")));
			}
			Enemy = GetWorld()->SpawnActor<ABaronEnemy>(BaronEnemyBP, EnemyPositions[i], FRotator(), SpawnParams);
			break;
		}
		
		if (Enemy)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("We got a thing.")));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("We got nothing.")));
		}
		EnemyReferences.Add(Enemy);
	}
}

void UBattleManager::LoadBattle()
{
	bBattleEnd = false;
	bPlayerTurn = true;
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("BattleMap")), true, "");
}

// Initialize the info we need as soon as the battle is loaded in
void UBattleManager::StartBattle()
{
	// we need to fetch the new playercontroller instance at the beginning of each battle
	// this way we make sure we have a valid playercontroller once the battle has loaded
	if (MainPlayerController == nullptr) {
		MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
	MainPlayerController->InitUI(Players, Enemies, bPlayerTurn, GetPlayer().Abilities);
	SpawnEnemies();
}

/**
* Prepares to segue to battle mode. It adds the players and enemies to the Battle Manager, and it initializes the Player Actions.
*/
void UBattleManager::PrepareForBattle(TArray<FEntityStruct> NewPlayers, TArray<FEntityStruct> NewEnemies)
{
	Players.Empty();
	Enemies.Empty();
	PlayerActions.Empty();
	EnemyReferences.Empty();
	Players = NewPlayers;
	Enemies = NewEnemies;
	TotalEXP = 0;
	TotalAP = 0;
	PlayerActions.Init(1, Players.Num());
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("Player Action array size: %d"), PlayerActions.Num()));*/
	bSelectingPlayer = true;
	bSelectingAbility = false;
	bSelectingTarget = false;
}

FEntityStruct UBattleManager::GetPlayer()
{
	return Players[PlayerIndex];
}

FEntityStruct UBattleManager::GetEnemy()
{
	return Enemies[0];
}

void UBattleManager::SetPlayerAbility()
{

	if (Players[PlayerIndex].bIsDead || PlayerActions[PlayerIndex] == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("Player %d is unavailable."), PlayerIndex));

		
	}	
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("Player Actions remaining: %d"), PlayerActions[PlayerIndex]));
		if (Players[PlayerIndex].Abilities[AbilityIndex].Cooldown > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("This ability is under cooldown.")));
		}
		else
		{
			HandlePlayerInput(Players[PlayerIndex].Abilities[AbilityIndex]); // Temporary. I want to be able to choose enemies first.
			// maybe just have a parameter here that takes in ability index and sets it instead of setting ability index in BattleWidget
		}
	}
}

/**
* Handle the enemy's input. Abilities have two properties that are checked: Move Types and Target Types. Once a player uses an ability with a cooldown, the ability goes on cooldown and cannot be used until the cooldown is up.
*/
void UBattleManager::HandlePlayerInput(FAbilityStruct SelectedAbility)
{
	// Load in the specified ability
	FAbilityStruct Ability = SelectedAbility;

	// Until I have support for UI buttons, the attacks will just be RANDOM or ALL

	switch (Ability.MoveType)
	{
		case EMoveTypeEnum::ATTACK:
			Cast<AMainCharacter>(MainPlayerController->GetPawn())->AnimateAttack();
			switch (Ability.TargetType)
			{
			case ETargetTypeEnum::SINGLE:
				// Missing Targeting
				HandleAttack(Ability, Players[PlayerIndex], Enemies[TargetIndex]);
				break;
			case ETargetTypeEnum::ALL:
				for (FEntityStruct &Enemy : Enemies)
				{
					HandleAttack(Ability, Players[PlayerIndex], Enemy);
				}
				break;
			case ETargetTypeEnum::RANDOM:
				TargetIndex = FMath::RandHelper(Enemies.Num());
				HandleAttack(Ability, Players[PlayerIndex], Enemies[TargetIndex]);
				break;
			}
			break;
		case EMoveTypeEnum::MAGIC:
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("Player casted a spell")));
			Cast<AMainCharacter>(MainPlayerController->GetPawn())->AnimateAttack();
			switch (Ability.TargetType)
			{
			case ETargetTypeEnum::ALLY:
				HandleMagic(Ability, Players[PlayerIndex], Players[TargetIndex]);
				break;
			case ETargetTypeEnum::ALLIES:
				for (FEntityStruct&Player : Players)
				{
					HandleMagic(Ability, Players[PlayerIndex], Player);
				}
				break;
			case ETargetTypeEnum::SINGLE:
				// Missing Targeting
				HandleMagic(Ability, Players[PlayerIndex], Enemies[TargetIndex]);
				break;
			case ETargetTypeEnum::ALL:
				for (FEntityStruct&Enemy : Enemies)
				{
					HandleMagic(Ability, Players[PlayerIndex], Enemy);
				}
				break;
			case ETargetTypeEnum::RANDOM:
				TargetIndex = FMath::RandHelper(Enemies.Num());
				HandleMagic(Ability, Players[PlayerIndex], Enemies[TargetIndex]);
				break;
			}
			break;
		case EMoveTypeEnum::HEAL:
			switch (Ability.TargetType)
			{
			case ETargetTypeEnum::ALLY:
				HandleHealing(Ability, Players[PlayerIndex], Players[TargetIndex]);
				break;
			case ETargetTypeEnum::ALLIES:
				for (FEntityStruct&Player : Players)
				{
					HandleHealing(Ability, Players[PlayerIndex], Player);
				}
				break;
			}
			break;
	}

	if (Players[PlayerIndex].Abilities[AbilityIndex].MaxCooldown > 0)
		Players[PlayerIndex].Abilities[AbilityIndex].Cooldown = Players[PlayerIndex].Abilities[AbilityIndex].MaxCooldown + 1; // The +1 is to make sure the next round is a cooldown round

	/*if (Players[PlayerIndex].HasteStacks > 1)
	{
		PlayerActions[PlayerIndex]++;
	}*/

	PlayerActions[PlayerIndex]--;

	PlayerIndex = 0;
	AbilityIndex = 0;
	TargetIndex = 0;

	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::PlayerTurn, 0.5f, false);
}

/**
* Handle the enemy's input. Abilities have two properties that are checked: Move Types and Target Types.
*/
void UBattleManager::HandleEnemyInput(FEntityStruct Source, FAbilityStruct SelectedAbility, int32 EnemyIndex)
{
	FAbilityStruct Ability = SelectedAbility;
	int EnemyTargetIndex = 0;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("Used %s"), *(Ability.AbilityName)));
	switch (Ability.MoveType)
	{
	case EMoveTypeEnum::ATTACK:
		if (EnemyReferences.IsValidIndex(EnemyIndex))
		{
			EnemyReferences[EnemyIndex]->Attack();
		}
		switch (Ability.TargetType)
		{
		case ETargetTypeEnum::SINGLE:
			// Missing Targeting
			HandleAttack(Ability, Source, Players[0]);
			break;
		case ETargetTypeEnum::ALL:
			for (FEntityStruct &Player: Players)
			{
				HandleAttack(Ability, Source, Player);
			}
			break;
		case ETargetTypeEnum::RANDOM:
			EnemyTargetIndex = FMath::RandHelper(Players.Num());
			HandleAttack(Ability, Source, Players[EnemyTargetIndex]);
			break;
		}
		break;
	case EMoveTypeEnum::MAGIC:
		if (EnemyReferences.IsValidIndex(EnemyIndex))
		{
			EnemyReferences[EnemyIndex]->Attack();
		}
		switch (Ability.TargetType)
		{
		case ETargetTypeEnum::ALLY:
			EnemyTargetIndex = FMath::RandHelper(Enemies.Num());
			HandleMagic(Ability, Source, Enemies[EnemyTargetIndex]);
			break;
		case ETargetTypeEnum::ALLIES:
			for (FEntityStruct &Enemy : Enemies)
			{
				HandleMagic(Ability, Source, Enemy);
			}
			break;
		case ETargetTypeEnum::SINGLE:
			// Missing Targeting
			HandleMagic(Ability, Source, Players[0]);
			break;
		case ETargetTypeEnum::ALL:
			for (FEntityStruct&Player : Players)
			{
				HandleMagic(Ability, Source, Player);
			}
			break;
		case ETargetTypeEnum::RANDOM:
			EnemyTargetIndex = FMath::RandHelper(Players.Num());
			HandleMagic(Ability, Source, Players[EnemyTargetIndex]);
			break;
		}
		break;
	case EMoveTypeEnum::HEAL:
		switch (Ability.TargetType)
		{
		case ETargetTypeEnum::ALLY:
			EnemyTargetIndex = FMath::RandHelper(Enemies.Num());
			HandleHealing(Ability, Source, Enemies[EnemyTargetIndex]);
			break;
		case ETargetTypeEnum::ALLIES:
			for (FEntityStruct&Enemy : Enemies)
			{
				HandleHealing(Ability, Source, Enemy);
			}
			break;
		}
		break;
	}
	//GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::EnemyToPlayerTransition, 0.5f, false);
}

/**
* Adjust the cooldowns of the player's abilities. As long as the cooldown is greater than 0, it will keep decrementing towards 0.
*/
void UBattleManager::AdjustCooldowns()
{
	for (FEntityStruct&Player : Players)
	{
		for (FAbilityStruct &Ability : Player.Abilities)
		{
			if (Ability.Cooldown > 0)
			{
				Ability.Cooldown--;
			}
		}
	}
}

/**
* Handle the damage of attacking abilities. Abilities can either be physical or magical based, relying on the target's defense or magic defense respectively. 
* In addition, abilities may have an elemental component which is further modified by the target's elemental resistances. If a target resists a 1000% elemental attack by over 100%, they will heal instead.
*/
void UBattleManager::HandleAttack(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target)
{
	bool HitsTarget = true;
	// If you are stunned, you cannot dodge. This does not use up STUN stacks.
	if (Target.StunStacks == 0)
	{
		float HitChance = (Source.Accuracy * Ability.Accuracy * (1 + Source.AccuracyBuff)) / (Target.Evasion * (1 + Target.EvasionBuff));
		if (HitChance < 1.0f)
		{
			HitsTarget = FMath::RandRange(0.f, 1.f) <= HitChance;
		}
	} 
	if (HitsTarget)
	{
		float Damage = 0;
		if (Ability.bIsPhysical)
		{
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("%s attack buff: %f"), *(Source.Name), Source.AttackBuff));
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Ability Power Array Size: %d"), Ability.Power.Num()));*/
			if (Ability.Power.IsValidIndex(Ability.Level - 1))
			{
				Damage = (Ability.Power[Ability.Level - 1] * Source.Attack * (1 + Source.AttackBuff)) / (Target.Defense * (1 + Target.DefenseBuff));
			}
			else
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("There is an error with physical Ability.Level - 1: %d"), Ability.Level));
			}
		}
		else
		{
			if (Ability.Power.IsValidIndex(Ability.Level - 1))
			{
				Damage = (Ability.Power[Ability.Level - 1] * Source.MagicAttack * (1 + Source.MagicAttackBuff)) / (Target.MagicDefense * (1 + Target.MagicDefenseBuff));
			}
			else
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("There is an error with magic Ability.Level - 1: %d"), Ability.Level));
			}
		}

		// Check for elements here
		if (Ability.ElementType != EElementTypeEnum::NONELEMENTAL)
		{
			float ElementalModifier = (1 - Ability.ElementalPercent) + (Ability.ElementalPercent * (1 - Target.ElementalResistances[(int32)Ability.ElementType]));

			// If a target is afflicted with CHILL, they take 50% more damage from ICE element attacks. Each time an attack is amplified, a stack is used. Fire damage removes CHILL
			if (Target.ChillStacks > 0)
			{
				if (Ability.ElementType == EElementTypeEnum::ICE)
				{
					ElementalModifier *= 1.5;
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("%s is chilled. Ice damage amplified. Chill stacks remaining: %d"), *(Target.Name), Target.ChillStacks));
					Target.ChillStacks--;
				}
				else if (Ability.ElementType == EElementTypeEnum::FIRE)
				{
					Target.ChillStacks = 0;
				}
			}
			Damage *= ElementalModifier;
		}
		Target.Health -= Target.bIsDefending ? Damage / 2 : Damage;

		HandleStatus(Ability.StatusType, Ability.StatusChance, Ability.StatusPower, Target);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Missed...")));
		}
	}
	Target.Health = FMath::Clamp(Target.Health, 0, Target.MaxHealth);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Cyan, FString::Printf(TEXT("%s: %f/%f"), *(Target.Name), Target.Health, Target.MaxHealth));
	MainPlayerController->UpdateBattleStats(Players, Enemies);
	if (Target.Health <= 0)
	{
		TotalEXP += Target.EXP;
		TotalAP += Target.AbilityPoints;
		Die(Target);
		// animate enemy death
		if (Target.EntityType == EEntityType::ENEMY)
		{
			if (EnemyReferences.IsValidIndex(TargetIndex))
				EnemyReferences[TargetIndex]->Die();
		}
		// somehow flag to delete in overworld
		// if (GameInstance)
		// 	GameInstance->RemoveEnemyAtLocation(Target.Location);
	}
}


/**
* Handle the buffs, debuffs, or status effects that come from an ability.
* Buffs and debuffs are constrained from -80% to 100%
* Status effects are constrained from 0 to 9 stacks
*/
void UBattleManager::HandleStatus(EStatusTypeEnum Status, float StatusChance, float StatusPower, FEntityStruct& Target)
{
	// No point in modifying a dead target.
	if (!Target.bIsDead)
	{
		// Remember to check the status chances.
		if (FMath::RandRange(0.f, 1.f) <= StatusChance)
		{
			float NewBuff;
			
			switch (Status)
			{
			case EStatusTypeEnum::ATTACKUP:

				NewBuff = Target.AttackBuff += StatusPower;
				NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
				Target.AttackBuff = NewBuff;
				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("%s got an attack boost of %f"), *(Target.Name), NewBuff));*/
				break;
			case EStatusTypeEnum::ATTACKDOWN:

				NewBuff = Target.AttackBuff -= StatusPower;
				NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
				Target.AttackBuff = NewBuff;
				break;
			case EStatusTypeEnum::MAGICATTACKUP:

				NewBuff = Target.MagicAttackBuff += StatusPower;
				NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
				Target.MagicAttackBuff = NewBuff;
				break;
			case EStatusTypeEnum::MAGICATTACKDOWN:

				NewBuff = Target.MagicAttackBuff -= StatusPower;
				NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
				Target.MagicAttackBuff = NewBuff;
				break;
			case EStatusTypeEnum::DEFENSEUP:

				NewBuff = Target.DefenseBuff += StatusPower;
				NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
				Target.DefenseBuff = NewBuff;
				break;
			case EStatusTypeEnum::DEFENSEDOWN:

				NewBuff = Target.DefenseBuff -= StatusPower;
				NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
				Target.DefenseBuff = NewBuff;
				break;
			case EStatusTypeEnum::MAGICDEFENSEUP:

				NewBuff = Target.MagicDefenseBuff += StatusPower;
				NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
				Target.MagicDefenseBuff = NewBuff;
				break;
			case EStatusTypeEnum::MAGICDEFENSEDOWN:

				NewBuff = Target.MagicDefenseBuff -= StatusPower;
				NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
				Target.MagicDefenseBuff = NewBuff;
				break;
			case EStatusTypeEnum::ACCURACYUP:

				NewBuff = Target.AccuracyBuff += StatusPower;
				NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
				Target.AccuracyBuff = NewBuff;
				break;
			case EStatusTypeEnum::ACCURACYDOWN:

				NewBuff = Target.AccuracyBuff -= StatusPower;
				NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
				Target.AccuracyBuff = NewBuff;
				break;
			case EStatusTypeEnum::EVASIONUP:

				NewBuff = Target.EvasionBuff += StatusPower;
				NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
				Target.EvasionBuff = NewBuff;
				break;
			case EStatusTypeEnum::EVASIONDOWN:

				NewBuff = Target.EvasionBuff -= StatusPower;
				NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
				Target.EvasionBuff = NewBuff;
				break;
			case EStatusTypeEnum::BURN:

				Target.BurnStacks += StatusPower;
				Target.BurnStacks = FMath::Clamp(Target.BurnStacks, 0, 9);
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Magenta, FString::Printf(TEXT("%s was burned with %d stacks."), *(Target.Name), Target.BurnStacks));
				break;
			case EStatusTypeEnum::CHILL:

				Target.ChillStacks += StatusPower;
				Target.ChillStacks = FMath::Clamp(Target.ChillStacks, 0, 9);
				break;
			case EStatusTypeEnum::STUN:

				Target.StunStacks += StatusPower;
				Target.StunStacks = FMath::Clamp(Target.StunStacks, 0, 9);
				break;
			case EStatusTypeEnum::POISON:

				Target.PoisonStacks += StatusPower;
				Target.PoisonStacks = FMath::Clamp(Target.PoisonStacks, 0, 9);
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Magenta, FString::Printf(TEXT("%s was poisoned with %d stacks."), *(Target.Name), Target.PoisonStacks));
				break;
			}
		}
	}
}

/**
* Adjust the temporary buffs and debuffs on an entity. As long as the (de)buffs are not 0, they reduce by 5% additively until they reach 0.
*/
void UBattleManager::AdjustBuffs(FEntityStruct& Target)
{
	// No point in modifying a dead target
	if (!Target.bIsDead)
	{
		if (Target.bIsDefending)
		{
			Target.bIsDefending = false;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("%s stopped defending."), *(Target.Name)));
		}
		if (FMath::Abs(Target.AttackBuff) != 0)
		{
			if (Target.AttackBuff > 0)
			{
				Target.AttackBuff -= 0.05;
			}
			else
			{
				Target.AttackBuff += 0.05;
			}
		}
		if (FMath::Abs(Target.MagicAttackBuff) != 0)
		{
			if (Target.MagicAttackBuff > 0)
			{
				Target.MagicAttackBuff -= 0.05;
			}
			else
			{
				Target.MagicAttackBuff += 0.05;
			}
		}
		if (FMath::Abs(Target.DefenseBuff) != 0)
		{
			if (Target.DefenseBuff > 0)
			{
				Target.DefenseBuff -= 0.05;
			}
			else
			{
				Target.DefenseBuff += 0.05;
			}
		}
		if (FMath::Abs(Target.MagicDefenseBuff) != 0)
		{
			if (Target.MagicDefenseBuff > 0)
			{
				Target.MagicDefenseBuff -= 0.05;
			}
			else
			{
				Target.MagicDefenseBuff += 0.05;
			}
		}
		if (FMath::Abs(Target.AccuracyBuff) != 0)
		{
			if (Target.AccuracyBuff > 0)
			{
				Target.AccuracyBuff -= 0.05;
			}
			else
			{
				Target.AccuracyBuff += 0.05;
			}
		}
		if (FMath::Abs(Target.EvasionBuff) != 0)
		{
			if (Target.EvasionBuff > 0)
			{
				Target.EvasionBuff -= 0.05;
			}
			else
			{
				Target.EvasionBuff += 0.05;
			}
		}
	}
}

/**
* Handle spells that do no damage to entitys but instead apply buffs, debuffs, or statuses.
*/
void UBattleManager::HandleMagic(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target)
{
	HandleStatus(Ability.StatusType, Ability.StatusChance, Ability.StatusPower, Target);
	MainPlayerController->UpdateBattleStats(Players, Enemies);
}

/**
* Adjust the status effects that decay at the end of a round. As long as a status stack is not equivalent to 0, they will move closer to 0.
*/
void UBattleManager::AdjustStatus(FEntityStruct& Target)
{
	if (Target.bIsDefending)
	{
		Target.bIsDefending = false;
	}
	if (Target.StunStacks > 1)
	{
		Target.StunStacks--;
	}
	if (Target.BurnStacks > 1)
	{
		Target.BurnStacks--;
	}
	if (Target.ChillStacks > 1)
	{
		Target.ChillStacks--;
	}
	if (Target.PoisonStacks > 1) 
	{
		Target.PoisonStacks--;
	}
}

/**
* Handle abilities that are meant to heal. Players gain health based on the ability's power. Enemies gain health based on their max health.
*/
void UBattleManager::HandleHealing(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target)
{
	if (!Target.bIsDead)
	{
		if (Source.EntityType == EEntityType::PLAYER || Target.EntityType == EEntityType::PLAYER)
		{
			Target.Health += Ability.Power[Ability.Level - 1] * 2;
			Target.Health = FMath::Clamp(Target.Health, 0, Target.MaxHealth);
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("Player healed %f health"), Ability.Power[Ability.Level - 1] * 2));
		}
		else
		{
			// I will need to add some sort of HitToHP modifier. This is based on the percent max hp.
			Target.Health += Target.MaxHealth * 0.3;
			Target.Health = FMath::Clamp(Target.Health, 0, Target.MaxHealth);
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("Enemy healed %f health"), Target.MaxHealth * 0.3));
		}
	}
	MainPlayerController->UpdateBattleStats(Players, Enemies);
}

/**
* Handle the damage that comes from being burned. Burns deal 100% fire damage at the end of a side's round turn. Burn damage is affected by fire resistance.
*/
void UBattleManager::HandleBurnDamage(FEntityStruct& Target)
{
	// No point in modifying a dead target
	if (!Target.bIsDead)
	{
		if (Target.BurnStacks > 0)
		{
			// Burns remove at least 7% of max health without accounting for resistance
			float Damage = Target.MaxHealth * 0.07 * Target.BurnStacks;
			// Check fire resistance
			Damage *= (1 - Target.ElementalResistances[(int32)EElementTypeEnum::FIRE]);
			Target.Health -= Damage;
			Target.BurnStacks--;

			Target.Health = FMath::Clamp(Target.Health, 0, Target.MaxHealth);
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("%s is on fire. %f damage taken"), *(Target.Name), Damage));
			MainPlayerController->UpdateBattleStats(Players, Enemies);
			if (Target.Health <= 0)
			{
				TotalEXP += Target.EXP;
				TotalAP += Target.AbilityPoints;
				Die(Target);
				if (Target.EntityType == EEntityType::ENEMY)
				{
					if (EnemyReferences.IsValidIndex(TargetIndex))
					{
						EnemyReferences[TargetIndex]->Die();
					}
				}
			}
		}
	}
}

/**
* Handle the damage that comes from being poisoned. Poison deals 100% poison damage at the end of a side's round turn. Poison damage is affected by poison resistance.
*/
void UBattleManager::HandlePoisonDamage(FEntityStruct& Target)
{
	// No point in modifying a dead target
	if (!Target.bIsDead)
	{
		if (Target.PoisonStacks > 0)
		{
			// Poison removes at least 5% of max health without accounting for resistance
			float Damage = Target.MaxHealth * 0.05 * Target.PoisonStacks;
			// Check poison resistance
			Damage *= (1 - Target.ElementalResistances[(int32)EElementTypeEnum::POISON]);
			Target.Health -= Damage;
			Target.PoisonStacks--;

			Target.Health = FMath::Clamp(Target.Health, 0, Target.MaxHealth);
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("%s is poisoned. %f damage taken"), *(Target.Name), Damage));
			MainPlayerController->UpdateBattleStats(Players, Enemies);
			if (Target.Health <= 0)
			{
				TotalEXP += Target.EXP;
				Die(Target);
				if (EnemyReferences.IsValidIndex(TargetIndex))
				{
					EnemyReferences[TargetIndex]->Die();
				}
			}
		}
	}
}

/**
* Start a round of combat. Player actions are replenished unless they are dead or stunned. If all party members are unable to act, then we transition to the enemy's turn
*/
void UBattleManager::StartRound()
{
	for (int i = 0; i < PlayerActions.Num(); i++)
	{
		if (!Players[i].bIsDead)
		{
			PlayerActions[i] = Players[i].StunStacks == 0 ? 1 : 0;
			if (Players[i].StunStacks > 0)
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("%s is stunned. Stun charges remaining: &d"), *(Players[i].Name), Players[i].StunStacks));
			}
		}
	}


	// TODO: Stun Handling if for some reason everyone is stunned
	/*bool bNotStunned = false;
	for (int i = 0; i < PlayerActions.Num(); i++)
	{
		if (PlayerActions[i] > 0)
		{
			bNotStunned = true;
		}
	}
	if (!bNotStunned)
	{

	}*/
	PlayerIndex = 0;
	AbilityIndex = 0;
	TargetIndex = 0;
	PlayerTurn(); // This should handle it as everyone has no actions.
}

/**
* End the current round. Adjusts the player's cooldowns, buffs, and statuses.
*/
void UBattleManager::EndRound()
{
	bool bPlayersDead = CheckPlayersIsDead();
	bool bEnemiesDead = CheckEnemiesIsDead();
	if (!bPlayersDead)
	{
		AdjustCooldowns();
		for (FEntityStruct& Player : Players)
		{
			if (!Player.bIsDead)
			{
				AdjustBuffs(Player);
				AdjustStatus(Player);
			}
		}
	}
	if (!bEnemiesDead)
	{
		for (FEntityStruct& Enemy : Enemies)
		{
			if (!Enemy.bIsDead)
			{
				AdjustBuffs(Enemy);
				AdjustStatus(Enemy);
			}
		}
	}
	bBattleEnd = bPlayersDead || bEnemiesDead;
	if (!bBattleEnd)
	{
		StartRound();
		MainPlayerController->UpdateBattleStats(Players, Enemies);
		MainPlayerController->UpdateTurnUI(bPlayerTurn);
	}
	else
	{
		if (bPlayersDead)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("The players are dead")));
		if (bEnemiesDead)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("The enemies are dead")));
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("BATTLE ENDED! RETURNING TO THE OVERWORLD...")));
		// let's add some buffer time for death animations to finish
		AMainCharacter* MainPlayer = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass()));
		MainPlayer->ResetCamera();
		GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::LeaveBattle, 1.0f, false);
	}
}

/**
* Transition to the enemy's turn. Burn damage happens at the end of a player's round turn.
*/
void UBattleManager::PlayerToEnemyTransition()
{
	for (FEntityStruct& Player : Players)
	{
		HandleBurnDamage(Player);
		HandlePoisonDamage(Player);
	}
	EnemyTurn();
}

/**
* Transition to the player's turn. Burn damage happens at the end of the enemy's round turn.
*/
void UBattleManager::EnemyToPlayerTransition()
{
	for (FEntityStruct& Enemy : Enemies)
	{
		HandleBurnDamage(Enemy);
		HandlePoisonDamage(Enemy);
	}
	bPlayerTurn = true;
	EndRound();
}

/**
* Check whether it is still currently the player's turn based on the number of actions left. If actions are exhausted, then we transition to the enemy's turn.
* Should be called each time a player makes an action.
*/
void UBattleManager::PlayerTurn()
{
	for (FEntityStruct Enemy : Enemies)
	{
		if (Enemy.Abilities.IsEmpty())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("%s is empty"), *(Enemy.EntityName)));
		}
	}

	bPlayerTurn = false;
	if (!CheckEnemiesIsDead())
	{
		for (int32 Actions : PlayerActions)
		{
			if (Actions > 0)
			{
				bPlayerTurn = true;
			}
		}
	}

	if (!bPlayerTurn)
	{
		GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::PlayerToEnemyTransition, 0.5f, false);
	}
	bSelectingPlayer = true;
	bSelectingAbility = false;
	bSelectingTarget = false;
}

/**
* Calculate what move an enemy should take before handling its input. Smarter enemies may have a different targeting strategy.
* For now, all enemies choose their abilities and targets randomly.
*/
void UBattleManager::EnemyTurn()
{
	MainPlayerController->UpdateTurnUI(bPlayerTurn); // Updating the turn UI
	for (int i = 0; i < Enemies.Num(); i++)
	{
		if (!Enemies[i].bIsDead)
		{
			if (Enemies[i].StunStacks == 0)
			{
				// We should not attack if there are no valid targets
				if (!CheckPlayersIsDead())
				{
					// Random Targetting behavior
					check(!Enemies[i].Abilities.IsEmpty());
					int EnemyAbilityIndex = FMath::RandHelper(Enemies[i].Abilities.Num());
					HandleEnemyInput(Enemies[i], Enemies[i].Abilities[EnemyAbilityIndex], i);
				}
				

				// Enemies are only allowed to act twice at most
				/*if (Enemy.HasteStacks > 1)
				{
					Enemy.HasteStacks = 0;
					if (!CheckPlayersIsDead())
					{
						EnemyAbilityIndex = FMath::RandHelper(Enemy.Abilities.Num());
						HandleEnemyInput(Enemy.Abilities[EnemyAbilityIndex]);
					}
				}*/
			}
		}
	}
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::EnemyToPlayerTransition, 0.5f, false);
}

/**
* Check whether all party members are dead. This should be checked whenever a party member dies.
*/
bool UBattleManager::CheckPlayersIsDead()
{
	bool bPlayersDead = true;
	for (FEntityStruct Player : Players)
	{
		if (!Player.bIsDead)
		{
			bPlayersDead = false;
		}
	}
	return bPlayersDead;
}

/**
* Check whether all enemies are dead. This should be checked whenever an enemy dies.
*/
bool UBattleManager::CheckEnemiesIsDead()
{
	bool bEnemiesDead = true;
	for (FEntityStruct Enemy : Enemies)
	{
		if (!Enemy.bIsDead) 
		{
			bEnemiesDead = false;
		}
	}
	return bEnemiesDead;
}

/**
* Call this function when the entity dies in battle. It removes all of the temporary buffs, debuffs, and statuses on the entity.
*/
void UBattleManager::Die(FEntityStruct& Target)
{
	if (Target.Health <= 0)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Magenta, FString::Printf(TEXT("%s is dead."), *(Target.Name)));
		Target.bIsDead = true;
		Target.bIsDefending = false;
		Target.AttackBuff = 0;
		Target.MagicAttackBuff = 0;
		Target.DefenseBuff = 0;
		Target.MagicDefenseBuff = 0;
		Target.AccuracyBuff = 0;
		Target.EvasionBuff = 0;
		Target.BurnStacks = 0;
		Target.ChillStacks = 0;
		Target.StunStacks = 0;
		Target.PoisonStacks = 0;
	}
}

/**
* Handle the distribution of EXP and AP. Players are given EXP based on the amount given from defeated enemies.
* Once players reach a threshold, they level up and the threshold increases.
* All players will receive EXP regardless if they are alive during the end of the battle.
*/
void UBattleManager::HandleEXP()
{
	for (FEntityStruct& Player : Players)
	{
		Player.EXP += TotalEXP;
		Player.AbilityPoints += TotalAP;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, FString::Printf(TEXT("%s has %d EXP"), *(Player.Name), Player.EXP));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, FString::Printf(TEXT("EXP Threshold is %d"), Player.EXPThreshold));
		while (Player.EXP >= Player.EXPThreshold)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%s has leveled up!"), *(Player.Name)));
			Player.EXP = Player.EXP - Player.EXPThreshold;
			Player.Level++;
			Player.Health = Player.MaxHealth;
			Player.EXPThreshold = (int32) (50 * FMath::Pow(1.39, Player.Level));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("New EXP threshold: %.0f."), 50 * FMath::Pow(1.39, Player.Level)));
			 // Handle overflow
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, FString::Printf(TEXT("%s has %d ability points"), *(Player.Name), Player.AbilityPoints));
	}
}

