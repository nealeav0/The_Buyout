// Fill out your copyright notice in the Description page of Project Settings.


#include "SeniorEnemy.h"

ASeniorEnemy::ASeniorEnemy()
{
}

void ASeniorEnemy::Attack()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Senior Attacks.")));
}

void ASeniorEnemy::Die()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Senior Dies.")));
}
