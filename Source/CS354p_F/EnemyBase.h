// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/DataTable.h"
#include "Ability.h"
#include "EntityBase.h"
#include "EnemyBase.generated.h"

USTRUCT(BlueprintType)
struct FEnemyStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityStruct> EnemyAbilites;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Accuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Evasion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EXP;

	UPROPERTY()
	float AttackBuff;

	UPROPERTY()
	float DefenseBuff;

	UPROPERTY()
	float EvasionBuff;

	UPROPERTY()
	float AccuracyBuff;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDefending;

	FEnemyStruct()
	{
		Name = FString(TEXT("Enemy"));
		Level = 1;
	}
	FEnemyStruct(FString NewName, int NewLevel)
	{
		Name = NewName;
		Level = NewLevel;
	}
};

UCLASS()
class CS354P_F_API AEnemyBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBase();

	UPROPERTY(EditAnywhere)
	FEntityStruct EnemyStats = FEntityStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	FEntityStruct GetEntityStruct();
};
