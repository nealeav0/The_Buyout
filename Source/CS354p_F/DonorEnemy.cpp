// Fill out your copyright notice in the Description page of Project Settings.


#include "DonorEnemy.h"

ADonorEnemy::ADonorEnemy()
{
}

void ADonorEnemy::Attack()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Donor Attacks.")));
}

void ADonorEnemy::Die()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Donor Dies.")));
}
