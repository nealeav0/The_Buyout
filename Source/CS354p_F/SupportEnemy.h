// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "SupportEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API ASupportEnemy : public AEnemyBase
{
	GENERATED_BODY()
	
public:

	ASupportEnemy();

	virtual void Attack() override;

	virtual void Die() override;
};