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

void UBattleManager::SelectPlayer(float Navigation)
{
	if (Navigation < 0)
	{
		PlayerIndex--;
		if (PlayerIndex < 0)
			PlayerIndex = Players.Num() - 1;
		// TODO: Make the PlayerIndex the next valid player if the current PlayerIndex has no actions left
	}
	else
	{
		PlayerIndex++;
		if (PlayerIndex > Players.Num() - 1)
			PlayerIndex = 0;
	}
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("Player Index: %d"), PlayerIndex));
}

void UBattleManager::SelectAbility(float Navigation)
{
	if (Navigation < 0)
	{
		AbilityIndex--;
		if (AbilityIndex < 0)
			AbilityIndex = Players[PlayerIndex].Abilities.Num() - 1;
	}
	else
	{
		AbilityIndex++;
		if (AbilityIndex > Players[PlayerIndex].Abilities.Num() - 1)
			AbilityIndex = 0;
	}
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("Ability Index: %d"), AbilityIndex));
}

void UBattleManager::SelectTarget(float Navigation)
{
	if (Navigation < 0)
	{
		TargetIndex--;
		if (TargetIndex < 0)
			AbilityIndex = Enemies.Num() - 1;
	}
	else
	{
		TargetIndex++;
		if (TargetIndex > Enemies.Num() - 1)
			TargetIndex = 0;
	}
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("Target Index: %d"), TargetIndex));
}

void UBattleManager::ConfirmSelection()
{
	if (bSelectingPlayer)
	{
		bSelectingPlayer = false;
		bSelectingAbility = true;
	}
	else if (bSelectingAbility)
	{
		float Cooldown = Players[PlayerIndex].Abilities[AbilityIndex].Cooldown;
		if (Cooldown > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("Turns left on Cooldown: % d"), Cooldown));
		}
		else
		{
			bSelectingAbility = false;
			bSelectingTarget = true;
		}
	}
	else if (bSelectingTarget)
	{
		bSelectingTarget = false;
		HandlePlayerInput(Players[PlayerIndex].Abilities[AbilityIndex]);
	}
}

void UBattleManager::DefendHandler()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("The Player is defending.")));
	Players[PlayerIndex].bIsDefending = true;
	PlayerActions[PlayerIndex]--;
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
	for (FEntityStruct& Player : Players)
	{
		Player.AttackBuff = 0;
		Player.MagicAttackBuff = 0;
		Player.DefenseBuff = 0;
		Player.MagicDefenseBuff = 0;
		Player.AccuracyBuff = 0;
		Player.EvasionBuff = 0;
		Player.BurnStacks = 0;
		Player.ChillStacks = 0;
		Player.StunStacks = 0;

		for (FAbilityStruct& Ability : Player.Abilities)
		{
			Ability.Cooldown = 0;
		}
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
	bSelectingPlayer = true;
	bSelectingAbility = false;
	bSelectingTarget = false;
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
					HandleMagic(Ability, Players[0], Enemy);
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
			TargetIndex = FMath::RandHelper(Enemies.Num());
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
			TargetIndex = FMath::RandHelper(Enemies.Num());
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
		float Damage;
		if (Ability.bIsPhysical)
		{
			Damage = (Ability.Power[Ability.Level - 1] * Source.Attack * (1 + Source.AttackBuff)) / (Target.Defense * (1 + Target.DefenseBuff));
		}
		else
		{
			Damage = (Ability.Power[Ability.Level - 1] * Source.MagicAttack * (1 + Source.MagicAttackBuff)) / (Target.MagicDefense * (1 + Target.MagicDefenseBuff));
		}

		// Check for elements here
		if (Ability.ElementType != EElementTypeEnum::NONELEMENTAL) 
		{
			float ElementalModifier = (1 - Ability.ElementalPercent) + (Ability.ElementalPercent * (1 - Target.ElementalResistances[(int32) Ability.ElementType]));

			// If a target is afflicted with CHILL, they take 50% more damage from ICE element attacks. Each time an attack is amplified, a stack is used. Fire damage removes CHILL
			if (Target.ChillStacks > 0)
			{
				if (Ability.ElementType == EElementTypeEnum::ICE)
				{
					ElementalModifier *= 1.5;
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
	MainPlayerController->UpdateBattleStats(GetPlayer(), GetEnemy());
	if (Target.Health <= 0)
	{
		TotalEXP += Target.EXP;
		// animate enemy death
		
		Target.bIsDead = true;
		CommonEnemy->Die();
		// somehow flag to delete in overworld
	}
}

void UBattleManager::HandleStatus(EStatusTypeEnum Status, float StatusChance, float StatusPower, FEntityStruct& Target)
{
	// No point in modifying a dead target.
	if (!Target.bIsDead)
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
			break;
		case EStatusTypeEnum::CHILL:

			Target.ChillStacks += StatusPower;
			Target.ChillStacks = FMath::Clamp(Target.ChillStacks, 0, 9);
			break;
		case EStatusTypeEnum::STUN:

			Target.StunStacks += StatusPower;
			Target.StunStacks = FMath::Clamp(Target.StunStacks, 0, 9);
			break;
		}
		
	}
}

void UBattleManager::AdjustBuffs(FEntityStruct& Target)
{
	// No point in modifying a dead target
	if (!Target.bIsDead)
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
	// No point in modifying a dead target
	if (!Target.bIsDead)
	{
		if (Target.BurnStacks > 0)
		{
			// Burns remove at least 7% of max health without accounting for resistance
			float Damage = Target.MaxHealth * 0.07 * Target.BurnStacks;
			// Check fire resistance
			Damage *= (1 - Target.ElementalResistances[(int32) EElementTypeEnum::FIRE]);
			Target.Health -= Damage;
			Target.BurnStacks--;

			Target.Health = FMath::Clamp(Target.Health, 0, Target.MaxHealth);
			MainPlayerController->UpdateBattleStats(GetPlayer(), GetEnemy());
			if (Target.Health <= 0)
			{
				TotalEXP += Target.EXP;
				Target.bIsDead = true;
				CommonEnemy->Die();
			}
		}
	}
}

void UBattleManager::StartRound()
{
	for (int i = 0; i < PlayerActions.Num(); i++)
	{
		if (!Players[i].bIsDead)
		{
			PlayerActions[i] = Players[i].StunStacks == 0 ? 1 : 0;		
		}
	}
	// TODO: Stun Handling if for some reason everyone is stunned
	PlayerIndex = 0;
}

void UBattleManager::EndRound()
{
	bSelectingPlayer = true;
	bSelectingAbility = false;
	bSelectingTarget = false;
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
			if (Player.StunStacks > 1)
			{
				Player.StunStacks--;
			}
		}
	}
	for (FEntityStruct Enemy : Enemies)
	{
		AdjustBuffs(Enemy);
		if (!Enemy.bIsDead) {
			bEnemiesDead = false;
			if (Enemy.StunStacks > 1)
			{
				Enemy.StunStacks--;
			}
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
		if (bPlayersDead)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("The players are dead")));
		if (bEnemiesDead)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("The enemies are dead")));
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("BATTLE ENDED! RETURNING TO THE OVERWORLD...")));
			// let's add some buffer time for death animations to finish
			GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::LeaveBattle, 1.0f, false);
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
	MainPlayerController->UpdateTurnUI(bPlayerTurn); // Updating the turn UI
	for (FEntityStruct Enemy : Enemies)
	{
		if (!Enemy.bIsDead)
		{
			if (Enemy.StunStacks == 0)
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

