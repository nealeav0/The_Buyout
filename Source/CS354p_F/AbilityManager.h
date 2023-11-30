// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Ability.h"
#include "AbilityManager.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API UAbilityManager : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void LearnAbility(FAbilityStruct& Ability);

	UFUNCTION(BlueprintCallable)
	void UpgradeAbility(FAbilityStruct& Ability);
};
