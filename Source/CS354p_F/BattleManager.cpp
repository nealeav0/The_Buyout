// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManager.h"
#include "MainPlayerController.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameInstance.h"

AMainPlayerController* MainPlayerController;
ACommonEnemy* CommonEnemy;
bool bEnemyDied;

UBattleManager::UBattleManager()
{
	// /Script/Engine.DataTable'/Game/Data/Player_Abilities.Player_Abilities'
	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerBPAbilities(TEXT("/Game/Data/Player_Abilities"));
	DataHandle.DataTable = PlayerBPAbilities.Object;

	MainPlayerController = nullptr;
	CommonEnemy = nullptr;
}

void UBattleManager::DefendHandler()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("The Player is defending.")));
	Players[0].bIsDefending = true;
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::PlayerToEnemyTransition, 0.5f, false);
}

void UBattleManager::EscapeHandler()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("The Player ran away.")));
	bBattleEnd = true;
	Cast<AMainCharacter>(MainPlayerController->GetPawn())->Escape();
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::LeaveBattle, 0.5f, false);
}

void UBattleManager::LeaveBattle() {
	// before we leave the battle, let's clean up a bit
	
	// We should tally up our EXP at the end of battle
	HandleEXP();

	// We should set all of the temporary stuff back to 0;
	Players[0].AttackBuff = 0;
	Players[0].DefenseBuff = 0;
	Players[0].AccuracyBuff = 0;
	Players[0].EvasionBuff = 0;

	for (FAbilityStruct& Ability : Players[0].Abilities)
	{
		Ability.Cooldown = 0;
	}

	MainPlayerController->CloseBattleUI();
	MainPlayerController = nullptr;
	CommonEnemy = nullptr;
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("OverworldMap")), true, "");
}

void UBattleManager::LoadBattle()
{
	bBattleEnd = false;
	bPlayerTurn = true;
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("BattleMap")), true, "");
}

// initialize the info we need as soon as the battle is loaded in
void UBattleManager::StartBattle()
{
	// we need to fetch the new playercontroller instance at the beginning of each battle
	// this way we make sure we have a valid playercontroller once the battle has loaded
	if (MainPlayerController == nullptr) {
		MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
	MainPlayerController->InitUI(GetPlayer(), GetEnemy(), bPlayerTurn, GetPlayer().Abilities);
	
	// also let's set the reference to the enemy we've loaded in
	CommonEnemy = Cast<ACommonEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), ACommonEnemy::StaticClass()));

	Rounds++;
}

void UBattleManager::PrepareForBattle(FEntityStruct Player, FEntityStruct Enemy)
{
	Players.Empty();
	Enemies.Empty();
	Players.Add(Player);
	Enemies.Add(Enemy);
	TotalEXP = 0;
}

FEntityStruct UBattleManager::GetPlayer()
{
	return Players[0];
}

FEntityStruct UBattleManager::GetEnemy()
{
	return Enemies[0];
}

void UBattleManager::SetPlayerAbility()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, Players[0].PlayerAbilities[AbilityIndex].AbilityName);

	if (Players[0].Abilities[AbilityIndex].Cooldown > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("This ability is under cooldown.")));
	}
	else 
	{
		
		 // I'm putting this here since round handling is a little scuffed.
		if (Players[0].Abilities[AbilityIndex].MaxCooldown > 0)
			Players[0].Abilities[AbilityIndex].Cooldown = Players[0].Abilities[AbilityIndex].MaxCooldown;
		HandlePlayerInput(Players[0].Abilities[AbilityIndex]); // Temporary. I want to be able to choose enemies first.
		// maybe just have a parameter here that takes in ability index and sets it instead of setting ability index in BattleWidget
	}
}

void UBattleManager::SetEnemyTarget()
{

}

// Choose an ability. Call HandlePlayerInput
void UBattleManager::HandlePlayerInput(FAbilityStruct SelectedAbility)
{
	// Load in the specified ability
	FAbilityStruct Ability = SelectedAbility;
	AbilityIndex = 0;

	// Until I have support for UI buttons, the attacks will just be RANDOM or ALL

	switch (Ability.MoveType)
	{
		case EMoveTypeEnum::ATTACK:
			Cast<AMainCharacter>(MainPlayerController->GetPawn())->AnimateAttack();
			switch (Ability.TargetType)
			{
			case ETargetTypeEnum::SINGLE:
				// Missing Targeting
				HandleAttack(Ability, Players[0], Enemies[0]);
				break;
			case ETargetTypeEnum::ALL:
				for (FEntityStruct &Enemy : Enemies)
				{
					HandleAttack(Ability, Players[0], Enemy);
				}
				break;
			case ETargetTypeEnum::RANDOM:
				int TargetIndex = FMath::RandHelper(Enemies.Num());
				HandleAttack(Ability, Players[0], Enemies[TargetIndex]);
				break;
			}
			break;
		case EMoveTypeEnum::MAGIC:
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("Player casted a spell")));
			Cast<AMainCharacter>(MainPlayerController->GetPawn())->AnimateAttack();
			switch (Ability.TargetType)
			{
			case ETargetTypeEnum::ALLY:
				HandleMagic(Ability, Players[0], Players[0]);
				break;
			case ETargetTypeEnum::ALLIES:
				for (FEntityStruct&Player : Players)
				{
					HandleMagic(Ability, Players[0], Player);
				}
				break;
			case ETargetTypeEnum::SINGLE:
				// Missing Targeting
				HandleMagic(Ability, Players[0], Enemies[0]);
				break;
			case ETargetTypeEnum::ALL:
				for (FEntityStruct&Enemy : Enemies)
				{
					HandleMagic(Ability, Players[0], Enemy);
				}
				break;
			case ETargetTypeEnum::RANDOM:
				int TargetIndex = FMath::RandHelper(Enemies.Num());
				HandleMagic(Ability, Players[0], Enemies[TargetIndex]);
				break;
			}
			break;
		case EMoveTypeEnum::HEAL:
			switch (Ability.TargetType)
			{
			case ETargetTypeEnum::ALLY:
				HandleHealing(Ability, Players[0], Players[0]);
				break;
			case ETargetTypeEnum::ALLIES:
				for (FEntityStruct&Player : Players)
				{
					HandleHealing(Ability, Players[0], Player);
				}
				break;
			}
			break;
	}
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::PlayerToEnemyTransition, 0.5f, false);
}

void UBattleManager::HandleEnemyInput(FAbilityStruct SelectedAbility)
{
	FAbilityStruct Ability = SelectedAbility;
	switch (Ability.MoveType)
	{
	case EMoveTypeEnum::ATTACK:
		CommonEnemy->Attack();
		switch (Ability.TargetType)
		{
		case ETargetTypeEnum::SINGLE:
			// Missing Targeting
			HandleAttack(Ability, Enemies[0], Players[0]);
			break;
		case ETargetTypeEnum::ALL:
			for (FEntityStruct &Player: Players)
			{
				HandleAttack(Ability, Enemies[0], Player);
			}
			break;
		case ETargetTypeEnum::RANDOM:
			int TargetIndex = FMath::RandHelper(Enemies.Num());
			HandleAttack(Ability, Enemies[0], Players[TargetIndex]);
			break;
		}
		break;
	case EMoveTypeEnum::MAGIC:
		CommonEnemy->Attack();
		switch (Ability.TargetType)
		{
		case ETargetTypeEnum::ALLY:
			HandleMagic(Ability, Enemies[0], Enemies[0]);
			break;
		case ETargetTypeEnum::ALLIES:
			for (FEntityStruct &Enemy : Enemies)
			{
				HandleMagic(Ability, Enemies[0], Enemy);
			}
			break;
		case ETargetTypeEnum::SINGLE:
			// Missing Targeting
			HandleMagic(Ability, Enemies[0], Players[0]);
			break;
		case ETargetTypeEnum::ALL:
			for (FEntityStruct&Player : Players)
			{
				HandleMagic(Ability, Enemies[0], Player);
			}
			break;
		case ETargetTypeEnum::RANDOM:
			int TargetIndex = FMath::RandHelper(Enemies.Num());
			HandleMagic(Ability, Enemies[0], Players[TargetIndex]);
			break;
		}
		break;
	case EMoveTypeEnum::HEAL:
		switch (Ability.TargetType)
		{
		case ETargetTypeEnum::ALLY:
			HandleHealing(Ability, Enemies[0], Enemies[0]);
			break;
		case ETargetTypeEnum::ALLIES:
			for (FEntityStruct&Enemy : Enemies)
			{
				HandleHealing(Ability, Enemies[0], Enemy);
			}
			break;
		}
		break;
	}
	//GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::EnemyToPlayerTransition, 0.5f, false);
}

// Keep in mind, this happens during the end of a round, not the player's turn.
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

void UBattleManager::HandleAttack(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target)
{
	bool HitsTarget = true;
	// If you are stunned, you cannot dodge
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
		float Damage = (Ability.Power[Ability.Level - 1] * Source.Attack * (1 + Source.AttackBuff)) / (Target.Defense * (1 + Target.DefenseBuff));
		// Check for elements here
		if (Ability.ElementType != EElementTypeEnum::NONELEMENTAL) 
		{
			Damage *= (1 - Ability.ElementalPercent) + (Ability.ElementalPercent * (1 - Target.ElementalResistances[(int32) Ability.ElementType]));
		}
		Target.Health = Target.bIsDefending ? Damage / 2 : Damage;
		HandleStatus(Ability.StatusType, Ability.StatusChance, Ability.StatusPower, Target);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Missed...")));
		}
	}
	MainPlayerController->UpdateBattleStats(GetPlayer(), GetEnemy());
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Enemy has %f remaining health"), Target.Health));
	if (Target.Health <= 0)
	{
		TotalEXP += Target.EXP;
		// animate enemy death
		Target.bIsDead = true;
		CommonEnemy->Die();
		// somehow flag to delete in overworld
	}
	 //GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::PlayerToEnemyTransition, 0.5f, false);
}

void UBattleManager::HandleStatus(EStatusTypeEnum Status, float StatusChance, float StatusPower, FEntityStruct& Target)
{

	float NewBuff;
	// Remember to check the status chances.
	switch (Status)
	{
	case EStatusTypeEnum::ATTACKUP:

		NewBuff = Target.AttackBuff += StatusPower;
		NewBuff = FMath::Clamp(NewBuff, -0.8f, 1.0f);
		Target.AttackBuff = NewBuff;
		break;
	case EStatusTypeEnum::ATTACKDOWN:

		NewBuff =  Target.AttackBuff -= StatusPower;
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
	}
}

void UBattleManager::AdjustBuffs(FEntityStruct& Target)
{
	if (Target.bIsDefending)
	{
		Target.bIsDefending = false;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("The Player stopped defending.")));
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


void UBattleManager::HandleMagic(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target)
{
	HandleStatus(Ability.StatusType, Ability.StatusChance, Ability.StatusPower, Target);
}

void UBattleManager::HandleHealing(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target)
{
	if (Source.EntityType == EEntityType::PLAYER)
	{
		Target.Health += Ability.Power[Ability.Level - 1] * 2;
		Target.Health = FMath::Clamp(Target.Health, 0, Target.MaxHealth);
	} 
	else
	{
		// I will need to add some sort of HitToHP modifier. This is based on the percent max hp.
		Target.Health += Target.MaxHealth * 0.3;
		Target.Health = FMath::Clamp(Target.Health, 0, Target.MaxHealth);
	}
}

void UBattleManager::HandleBurnDamage(FEntityStruct& Target)
{
	if (!Target.bIsDead)
	{
		if (Target.BurnStacks > 0)
		{
			float Damage = Target.MaxHealth * 0.0007 * Target.BurnStacks;
			// Check fire resistance
		}
	}
}

void UBattleManager::StartRound()
{
	for (int i = 0; i < PlayerActions.Num(); i++)
	{
		if (!Players[i].bIsDead)
		{
			PlayerActions[i] = 1;
		}
	}
	PlayerIndex = 0;
}

void UBattleManager::EndRound()
{
	bool bPlayersDead = true;
	bool bEnemiesDead = true;
	AdjustCooldowns();
	for (FEntityStruct Player : Players)
	{
		AdjustBuffs(Player);
		Player.bIsDefending = false;
		if (!Player.bIsDead)
		{
			bPlayersDead = false;
		}
	}
	for (FEntityStruct Enemy : Enemies)
	{
		AdjustBuffs(Enemy);
		if (!Enemy.bIsDead) {
			bEnemiesDead = false;
		}
	}
	bBattleEnd = bPlayersDead || bEnemiesDead;
	if (!bBattleEnd)
	{
		MainPlayerController->UpdateBattleStats(GetPlayer(), GetEnemy());
		MainPlayerController->UpdateTurnUI(bPlayerTurn);
		StartRound();
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("BATTLE ENDED! RETURNING TO THE OVERWORLD...")));
		if (Enemies[0].bIsDead)
		{
			// let's add some buffer time for death animations to finish
			GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::LeaveBattle, 1.0f, false);
		}
		else 
		{
			GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::LeaveBattle, 0.5f, false);
		}
	}
}

void UBattleManager::PlayerToEnemyTransition()
{
	
	for (FEntityStruct Player : Players)
	{
		HandleBurnDamage(Player);
	}
	EnemyTurn();
}

void UBattleManager::EnemyToPlayerTransition()
{
	for (FEntityStruct Enemy : Enemies)
	{
		HandleBurnDamage(Enemy);
	}
	bPlayerTurn = true;
	EndRound();
}

void UBattleManager::PlayerTurn()
{
	bPlayerTurn = false;
	for (int32 Actions : PlayerActions)
	{
		if (Actions > 0)
		{
			bPlayerTurn = true;
		}
	}

	if (!bPlayerTurn)
	{
		GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::PlayerToEnemyTransition, 0.5f, false);
	}	
}

void UBattleManager::EnemyTurn()
{
	for (FEntityStruct Enemy : Enemies)
	{
		if (!Enemy.bIsDead)
		{
			// Random Targetting behavior
			int EnemyAbilityIndex = FMath::RandHelper(Enemy.Abilities.Num());
			HandleEnemyInput(Enemy.Abilities[EnemyAbilityIndex]);

			// Enemies are only allowed to act twice at most
			/*if (Enemy.HasteStacks > 1)
			{
				Enemy.HasteStacks = 0;
				EnemyAbilityIndex = FMath::RandHelper(Enemy.Abilities.Num());
				HandleEnemyInput(Enemy.Abilities[EnemyAbilityIndex]);
			}*/
		}
	}
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::EnemyToPlayerTransition, 0.5f, false);
}

void UBattleManager::HandleEXP()
{
	for (FEntityStruct& Player : Players)
	{
		Player.EXP += TotalEXP;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("Player has %f EXP"), Player.EXP));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("EXP Threshold is %f"), EXPThreshold));
		if (Player.EXP >= EXPThreshold)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("Player has leveled up!")));
			Player.Level++;
			Player.EXP = FMath::Fmod(Player.EXP, EXPThreshold); // Handle overflow
			EXPThreshold *= 2; //Update Threshold
		}
	}
}

