// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityManager.h"

void UAbilityManager::LearnAbility(FAbilityStruct& Ability)
{
	if (!Ability.bIsLearned)
	{
		Ability.bIsLearned = true;
		Ability.Level = 1;
	}
}

void UAbilityManager::UpgradeAbility(FAbilityStruct& Ability)
{
	if (Ability.bIsLearned)
	{
		if (Ability.Level < Ability.MaxLevel)
		{
			Ability.Level++;
		}
	}
}
