// Fill out your copyright notice in the Description page of Project Settings.


#include "BaronEnemy.h"

ABaronEnemy::ABaronEnemy()
{
}

void ABaronEnemy::Attack()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Baron Attacks.")));
}

void ABaronEnemy::Die()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Baron Dies.")));
}
