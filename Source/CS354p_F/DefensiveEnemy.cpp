// Fill out your copyright notice in the Description page of Project Settings.


#include "DefensiveEnemy.h"

ADefensiveEnemy::ADefensiveEnemy()
{
}

void ADefensiveEnemy::Attack()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Defensive Attacks.")));
}

void ADefensiveEnemy::Die()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Defensive Dies.")));
}
