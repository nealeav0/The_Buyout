// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "DonorEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API ADonorEnemy : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	ADonorEnemy();

	virtual void Attack() override;

	virtual void Die() override;
};
