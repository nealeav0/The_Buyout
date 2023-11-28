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

void UBattleManager::StartTurn()
{	
	bEnemyDied = false;
	
	// Enemy chooses a target
	// Target = Player
	// Attack = Bash
	// AttackHandler()
	// Enemy chooses an attack/action
	// Enemy calls EndTurn();
	if (!bPlayerTurn)
	{
		int EnemyIndex = FMath::RandHelper(Enemies[0].EnemyAbilites.Num());
		HandleEnemyInput(Enemies[0].EnemyAbilites[EnemyIndex]);
	}
	else
	{
		if (Players[0].bIsDefending)
		{
			Players[0].bIsDefending = false;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("The Player stopped defending.")));
		}
		// if (GEngine)
			// GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("It's the Player's turn.")));
	}
	// Stops at this point as it is the player's turn
	for (FPlayerStruct &Player : Players)
	{
		AdjustBuffs(Player);
	}
	for (FEnemyStruct &Enemy : Enemies)
	{
		AdjustBuffs(Enemies[0]);
	}
	
}

/*

=== player handlers ===

*/
// @params The instigator, the attack power of attack, and target
// I need to add handling to who started the attack
//
void UBattleManager::AttackHandler(FAbilityStruct Ability, FEnemyStruct &Target)
{
	// If it hits
	bool HitsTarget = true;
	float HitChance = (Players[0].Accuracy * Ability.Accuracy * (1 + Players[0].AccuracyBuff)) / (Target.Evasion * (1 + Target.EvasionBuff));
	// if (GEngine)
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Player has a hit chance of %f"), HitChance));
	if (HitChance < 1.0f)
	{
		HitsTarget = FMath::RandRange(0.f, 1.f) <= HitChance;
	}
	if (HitsTarget)
	{
		float Damage = (Ability.Power * Players[0].Attack * (1 + Players[0].AttackBuff)) / (Target.Defense * (1 + Target.DefenseBuff));
		Target.Health -= Damage;
		// if (GEngine)
		// {
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Player did %f damage"), Damage));
		// }
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Player missed...")));
			
		}		
	}

	// Here's will we handle the secondary status effects.
	/*if (Ability.StatusType != EStatusTypeEnum::NONE)*/
		HandleStatus(Ability.StatusType, Ability.StatusPower, Target);
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Enemy's AttackBuff is %f"), Target.AttackBuff));
	
	/* Blake's Implementation
	float Damage =  (Power * Player.Attack * Player.AtkBuf) / Target.Defense;
	Enemy.Health -= Damage;
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("Player did %f damage"), Damage));
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("Enemy has %f remaining health"), Enemy.Health));
	}
	*/

	// TimerManager()bn 
	// Update UI
	MainPlayerController->UpdateBattleStats(GetPlayer(), GetEnemy());
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Enemy has %f remaining health"), Target.Health));
	if (Target.Health <= 0)
	{
		TotalEXP += Target.EXP;
		bBattleEnd = true;
		// animate enemy death
		bEnemyDied = true;
		CommonEnemy->Die();
		// somehow flag to delete in overworld
	}
	// GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::EndTurn, 0.5f, false);
}


void UBattleManager::DefendHandler()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("The Player is defending.")));
	Players[0].bIsDefending = true;
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::EndTurn, 0.5f, false);
}

void UBattleManager::EscapeHandler()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("The Player ran away.")));
	bBattleEnd = true;
	Cast<AMainCharacter>(MainPlayerController->GetPawn())->Escape();
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::EndTurn, 0.5f, false);
}

/*

=== enemy handlers ===

*/

void UBattleManager::AttackHandler(FAbilityStruct Ability, FPlayerStruct &Target)
{
	/*
	Attack : Does the normal attack ability
	Ability : OnButtonPress open another menu which has a list of buttons that contain the names of the abilities that player has. OnMouseHover, then we show a menu with a description.
	Defend : Defend
	Order : Switch to a character who has not taken their turn yet.
	
	*/
	// It it hits
	bool HitsTarget = true;
	float HitChance = (Enemies[0].Accuracy * Ability.Accuracy * (1 + Enemies[0].AccuracyBuff)) / (Target.Evasion * (1 + Target.EvasionBuff));
	// if (GEngine)
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Enemy has a hit chance of %f"), HitChance));
	if (HitChance < 1.0f) 
	{
		HitsTarget = FMath::RandRange(0.f, 1.f) <= HitChance;
	}
	if (HitsTarget)
	{
		float Damage = (Ability.Power * Enemies[0].Attack * (1 + Enemies[0].AttackBuff)) / (Target.Defense * (1 + Target.DefenseBuff));
		if (Target.bIsDefending)
		{
			// if (GEngine)
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Defending")));
			Target.Health -= Damage / 2;
			// if (GEngine)
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Enemy did %f damage"), Damage / 2));
		}
		else
		{
			Target.Health -= Damage;
			// if (GEngine)
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Enemy did %f damage"), Damage));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Enemy missed...")));
		}
	}
	// Play Animation
	// TimerManager();
	// Update UI
	MainPlayerController->UpdateBattleStats(GetPlayer(), GetEnemy());
	// if (GEngine)
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Player has %f remaining health"), Target.Health));
	if (Target.Health <= 0)
	{
		bBattleEnd = true;
	}
	//GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::EndTurn, 0.5f, false);
}

void UBattleManager::EndTurn()
{
	bPlayerTurn = !bPlayerTurn;
	// if (GEngine)
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Player's Turn: %B."), bPlayerTurn));
	if (!bBattleEnd) {
		MainPlayerController->UpdateBattleStats(GetPlayer(), GetEnemy());
		MainPlayerController->UpdateTurnUI(bPlayerTurn);
		if(bPlayerTurn) {
			// GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, FString::Printf(TEXT("~ now player turn ~")));
			// GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, FString::Printf(TEXT("Player AtkBuf: %f"), Players[0].AttackBuff));
			// GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, FString::Printf(TEXT("Player DefBuf: %f"), Players[0].DefenseBuff));
			GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::StartTurn, 0.5f, false);
		} else {
			GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::StartTurn, 1.5f, false);
		}
		
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("BATTLE ENDED! RETURNING TO THE OVERWORLD...")));

		
		if (bEnemyDied) {
			// let's add some buffer time for death animations to finish
			GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::LeaveBattle, 1.0f, false);
		} else {
			GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::LeaveBattle, 0.5f, false);
		}
	}
	// Prepare to jump to overworld.
}

/*

=== general functions ===

*/

void UBattleManager::LeaveBattle() {
	// before we leave the battle, let's clean up a bit
	
	// We should tally up our EXP at the end of battle
	HandleEXP();

	// We should set all of the temporary stuff back to 0;
	Players[0].AttackBuff = 0;
	Players[0].DefenseBuff = 0;
	Players[0].AccuracyBuff = 0;
	Players[0].EvasionBuff = 0;

	for (FAbilityStruct& Ability : Players[0].PlayerAbilities)
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
	bAbilityChosen = false;
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
	MainPlayerController->InitUI(GetPlayer(), GetEnemy(), bPlayerTurn, GetPlayer().PlayerAbilities);
	
	// also let's set the reference to the enemy we've loaded in
	CommonEnemy = Cast<ACommonEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), ACommonEnemy::StaticClass()));

	Rounds++;
}

void UBattleManager::PrepareForBattle(FPlayerStruct PlayerStruct, FEnemyStruct EnemyStruct)
{
	Players.Empty();
	Enemies.Empty();
	Players.Add(PlayerStruct);
	/*if (Players[0].PlayerAbilities.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Array is empty")));
	}*/
	Enemies.Add(EnemyStruct);
}

FPlayerStruct UBattleManager::GetPlayer()
{
	return Players[0];
}

FEnemyStruct UBattleManager::GetEnemy()
{
	return Enemies[0];
}

void UBattleManager::SetPlayerAbility()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, Players[0].PlayerAbilities[AbilityIndex].AbilityName);

	if (Players[0].PlayerAbilities[AbilityIndex].Cooldown > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("This ability is under cooldown.")));
	}
	else 
	{
		bAbilityChosen = true;
		AdjustCooldowns(); // I'm putting this here since round handling is a little scuffed.
		if (Players[0].PlayerAbilities[AbilityIndex].MaxCooldown > 0)
			Players[0].PlayerAbilities[AbilityIndex].Cooldown = Players[0].PlayerAbilities[AbilityIndex].MaxCooldown;
		HandlePlayerInput(Players[0].PlayerAbilities[AbilityIndex]); // Temporary. I want to be able to choose enemies first.
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
	bAbilityChosen = false;
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
				AttackHandler(Ability, Enemies[0]);
				break;
			case ETargetTypeEnum::ALL:
				for (FEnemyStruct &Enemy : Enemies)
				{
					AttackHandler(Ability, Enemy);
				}
				break;
			case ETargetTypeEnum::RANDOM:
				int TargetIndex = FMath::RandHelper(Enemies.Num());
				AttackHandler(Ability, Enemies[TargetIndex]);
				break;
			}
			break;
		case EMoveTypeEnum::MAGIC:
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("Player casted a spell")));
			Cast<AMainCharacter>(MainPlayerController->GetPawn())->AnimateAttack();
			switch (Ability.TargetType)
			{
			case ETargetTypeEnum::ALLY:
				HandleMagic(Ability, Players[0]);
				break;
			case ETargetTypeEnum::ALLIES:
				for (FPlayerStruct &Player : Players)
				{
					HandleMagic(Ability, Player);
				}
				break;
			case ETargetTypeEnum::SINGLE:
				// Missing Targeting
				HandleMagic(Ability, Enemies[0]);
				break;
			case ETargetTypeEnum::ALL:
				for (FEnemyStruct &Enemy : Enemies)
				{
					HandleMagic(Ability, Enemy);
				}
				break;
			case ETargetTypeEnum::RANDOM:
				int TargetIndex = FMath::RandHelper(Enemies.Num());
				HandleMagic(Ability, Enemies[TargetIndex]);
				break;
			}
			break;
		case EMoveTypeEnum::HEAL:
			switch (Ability.TargetType)
			{
			case ETargetTypeEnum::ALLY:
				HandleHealing(Ability, Players[0]);
				break;
			case ETargetTypeEnum::ALLIES:
				for (FPlayerStruct &Player : Players)
				{
					HandleHealing(Ability, Player);
				}
				break;
			}
			break;
	}
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::EndTurn, 0.5f, false);
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
			AttackHandler(Ability, Players[0]);
			break;
		case ETargetTypeEnum::ALL:
			for (FPlayerStruct &Player: Players)
			{
				AttackHandler(Ability, Player);
			}
			break;
		case ETargetTypeEnum::RANDOM:
			int TargetIndex = FMath::RandHelper(Enemies.Num());
			AttackHandler(Ability, Players[TargetIndex]);
			break;
		}
		break;
	case EMoveTypeEnum::MAGIC:
		CommonEnemy->Attack();
		switch (Ability.TargetType)
		{
		case ETargetTypeEnum::ALLY:
			HandleMagic(Ability, Enemies[0]);
			break;
		case ETargetTypeEnum::ALLIES:
			for (FEnemyStruct &Enemy : Enemies)
			{
				HandleMagic(Ability, Enemy);
			}
			break;
		case ETargetTypeEnum::SINGLE:
			// Missing Targeting
			HandleMagic(Ability, Players[0]);
			break;
		case ETargetTypeEnum::ALL:
			for (FPlayerStruct &Player : Players)
			{
				HandleMagic(Ability, Player);
			}
			break;
		case ETargetTypeEnum::RANDOM:
			int TargetIndex = FMath::RandHelper(Enemies.Num());
			HandleMagic(Ability, Players[TargetIndex]);
			break;
		}
		break;
	case EMoveTypeEnum::HEAL:
		switch (Ability.TargetType)
		{
		case ETargetTypeEnum::ALLY:
			HandleHealing(Ability, Enemies[0]);
			break;
		case ETargetTypeEnum::ALLIES:
			for (FEnemyStruct &Enemy : Enemies)
			{
				HandleHealing(Ability, Enemy);
			}
			break;
		}
		break;
	}
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UBattleManager::EndTurn, 0.5f, false);
}

void UBattleManager::HandleStatus(EStatusTypeEnum Status, float StatusPower, FPlayerStruct& Target)
{
	switch (Status)
	{
	case EStatusTypeEnum::ATTACKUP:
		Target.AttackBuff += StatusPower;
		break;
	case EStatusTypeEnum::ATTACKDOWN:
		Target.AttackBuff -= StatusPower;
		break;
	case EStatusTypeEnum::DEFENSEUP:
		Target.DefenseBuff += StatusPower;
		break;
	case EStatusTypeEnum::DEFENSEDOWN:
		Target.DefenseBuff -= StatusPower;
		break;
	case EStatusTypeEnum::ACCURACYUP:
		Target.AccuracyBuff += StatusPower;
		break;
	case EStatusTypeEnum::ACCURACYDOWN:
		Target.AccuracyBuff -= StatusPower;
		break;
	case EStatusTypeEnum::EVASIONUP:
		Target.EvasionBuff += StatusPower;
		break;
	case EStatusTypeEnum::EVASIONDOWN:
		Target.EvasionBuff -= StatusPower;
		break;
	}
}

void UBattleManager::HandleStatus(EStatusTypeEnum Status, float StatusPower, FEnemyStruct &Target)
{
	switch (Status)
	{
	case EStatusTypeEnum::ATTACKUP:
		Target.AttackBuff += StatusPower;
		break;
	case EStatusTypeEnum::ATTACKDOWN:
		Target.AttackBuff -= StatusPower;
		break;
	case EStatusTypeEnum::DEFENSEUP:
		Target.DefenseBuff += StatusPower;
		break;
	case EStatusTypeEnum::DEFENSEDOWN:
		Target.DefenseBuff -= StatusPower;
		break;
	case EStatusTypeEnum::ACCURACYUP:
		Target.AccuracyBuff += StatusPower;
		break;
	case EStatusTypeEnum::ACCURACYDOWN:
		Target.AccuracyBuff -= StatusPower;
		break;
	case EStatusTypeEnum::EVASIONUP:
		Target.EvasionBuff += StatusPower;
		break;
	case EStatusTypeEnum::EVASIONDOWN:
		Target.EvasionBuff -= StatusPower;
		break;
	}
}

void UBattleManager::HandleMagic(FAbilityStruct Ability, FPlayerStruct& Target)
{
	// Magic abilities include the ability to target yourself.
	/*if (Ability.TargetType == ETargetTypeEnum::ALLY || Ability.TargetType == ETargetTypeEnum::ALLIES)
	{*/
		HandleStatus(Ability.StatusType, Ability.StatusPower, Target);
	
}

void UBattleManager::HandleMagic(FAbilityStruct Ability, FEnemyStruct& Target)
{
	HandleStatus(Ability.StatusType, Ability.StatusPower, Target);
}

void UBattleManager::HandleHealing(FAbilityStruct Ability, FPlayerStruct &Target)
{
	Target.Health += Ability.Power * 2;
	Target.Health = FMath::Clamp(Target.Health, 0, Target.MaxHealth);
}

void UBattleManager::HandleHealing(FAbilityStruct Ability, FEnemyStruct& Target)
{
	// I will need to add some sort of HitToHP modifier. This is based on the percent max hp.
	Target.Health += Target.MaxHealth * 0.3;
}

void UBattleManager::AdjustBuffs(FPlayerStruct& Target)
{
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

void UBattleManager::AdjustBuffs(FEnemyStruct& Target)
{
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

// Keep in mind, this happens during the end of a round, not the player's turn.
void UBattleManager::AdjustCooldowns()
{
	for (FPlayerStruct &Player : Players)
	{
		for (FAbilityStruct &Ability : Player.PlayerAbilities)
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
	float HitChance = (Source.Accuracy * Ability.Accuracy * (1 + Source.AccuracyBuff)) / (Target.Evasion * (1 + Target.EvasionBuff));
	if (HitChance < 1.0f)
	{
		HitsTarget = FMath::RandRange(0.f, 1.f) <= HitChance;
	}
	if (HitsTarget)
	{
		float Damage = (Ability.Power * Source.Attack * (1 + Source.AttackBuff)) / (Target.Defense * (1 + Target.DefenseBuff));
		// Check for elements here
		if (Target.bIsDefending)
		{
			Target.Health -= Damage / 2;
		}
		else
		{
			Target.Health -= Damage;
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Missed...")));
		}
	}
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
}

void UBattleManager::HandleHealing(FAbilityStruct Ability, FEntityStruct Source, FEntityStruct& Target)
{
	if (Source.EntityType == EEntityType::PLAYER)
	{
		Target.Health += Ability.Power * 2;
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

void UBattleManager::HandleEXP()
{
	for (FPlayerStruct& Player : Players)
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

