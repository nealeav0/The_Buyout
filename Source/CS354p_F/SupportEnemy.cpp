// Fill out your copyright notice in the Description page of Project Settings.


#include "SupportEnemy.h"

ASupportEnemy::ASupportEnemy()
{
}

void ASupportEnemy::Attack()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Support Attacks.")));
}

void ASupportEnemy::Die()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Support Dies.")));
}
