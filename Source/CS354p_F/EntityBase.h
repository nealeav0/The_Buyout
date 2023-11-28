// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "Ability.h"
#include "EntityBase.generated.h"

UENUM()
enum class EEntityType
{
	PLAYER,
	ENEMY,
	NPC
};

USTRUCT(BlueprintType)
struct FEntityStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY()
	FString Name;

	UPROPERTY()
	EEntityType EntityType;

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	float EXP;

	UPROPERTY()
	bool bIsDead;

	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	float Health;

	UPROPERTY()
	float Attack;

	UPROPERTY()
	float Defense;

	UPROPERTY()
	float MagicAttack;

	UPROPERTY()
	float MagicDefense;

	UPROPERTY()
	float Accuracy;

	UPROPERTY()
	float Evasion;

	UPROPERTY()
	float AttackBuff;

	UPROPERTY()
	float DefenseBuff;

	UPROPERTY()
	float MagicAttackBuff;

	UPROPERTY()
	float MagicDefenseBuff;

	UPROPERTY()
	float AccuracyBuff;

	UPROPERTY()
	float EvasionBuff;

	UPROPERTY()
	float FireResistance;

	UPROPERTY()
	float IceResistance;

	UPROPERTY()
	float ThunderResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDefending;

	// Status Struct Stuff

	UPROPERTY()
	int32 BurnStacks;

	UPROPERTY()
	int32 ChillStacks;

	UPROPERTY()
	int32 StunStacks;

	FEntityStruct()
	{
		Name = FString(TEXT("NPC"));
		EntityType = EEntityType::NPC;
		Level = 1;
		bIsDead = false;
		MaxHealth = 0.f;
		Health = MaxHealth;
		Attack = 0.f;
		Defense = 0.f;
		MagicAttack = 0.f;
		MagicDefense = 0.f;
		AttackBuff = 0.f;
		DefenseBuff = 0.f;
		AccuracyBuff = 0.f;
		EvasionBuff = 0.f;
		FireResistance = 0.f;
		IceResistance = 0.f;
		ThunderResistance = 0.f;
		Location = FVector(0, 0, 0);
		bIsDefending = false;
		BurnStacks = 0;
		ChillStacks = 0;
		StunStacks = 0;
	}
};

/**
 * 
 */
UCLASS()
class CS354P_F_API UEntityBase : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};
