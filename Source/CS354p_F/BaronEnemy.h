// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "BaronEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API ABaronEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	ABaronEnemy();
	
	virtual void Attack() override;

	virtual void Die() override;
};
