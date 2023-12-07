// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "DefensiveEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API ADefensiveEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:

	ADefensiveEnemy();
	
	virtual void Attack() override;

	virtual void Die() override;
};
