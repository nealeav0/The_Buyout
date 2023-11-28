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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEntityType EntityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityStruct> Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicDefense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Accuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Evasion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IceResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThunderResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicAttackBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefenseBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicDefenseBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AccuracyBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EvasionBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDefending;

	// Status Struct Stuff

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BurnStacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ChillStacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
