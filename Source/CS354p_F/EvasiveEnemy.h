// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EvasiveEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API AEvasiveEnemy : public AEnemyBase
{
	GENERATED_BODY()
	
public:

	AEvasiveEnemy();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Attack() override;

	UFUNCTION()
	virtual void Die() override;
};
