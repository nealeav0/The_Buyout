// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "SeniorEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API ASeniorEnemy : public AEnemyBase
{
	GENERATED_BODY()
	
public:

	ASeniorEnemy();

	virtual void Attack() override;

	virtual void Die() override;
};
