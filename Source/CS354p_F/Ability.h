// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Ability.generated.h"

UENUM()
enum class ETargetTypeEnum
{
	ALLY,
	ALLIES,
	SINGLE,
	ALL,
	RANDOM
};

UENUM()
enum class EMoveTypeEnum
{
	ATTACK,
	MAGIC,
	HEAL
};

UENUM()
enum class EStatusTypeEnum
{
	NONE,
	ATTACKUP,
	ATTACKDOWN,
	DEFENSEUP,
	DEFENSEDOWN,
	ACCURACYUP,
	ACCURACYDOWN,
	EVASIONUP,
	EVASIONDOWN
};

USTRUCT(BlueprintType)
struct FAbilityStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoveTypeEnum MoveType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETargetTypeEnum TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Accuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatusTypeEnum StatusType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StatusPower;

	FAbilityStruct() 
	{
		AbilityName = FString(TEXT("Hit"));
		Description = FString(TEXT("ATTACK a SINGLE RANDOM target"));
		MoveType = EMoveTypeEnum::ATTACK;
		TargetType = ETargetTypeEnum::RANDOM;
		Power = 30.0f;
		Accuracy = 1.0f;
		MaxCooldown = 0;
		Cooldown = 0;
		StatusType = EStatusTypeEnum::NONE;
		StatusPower = 0;
	}

	/**
	* This is the struct used to create abilities. Ideally, the data should come from a CSV list, so we don't have to keep typing it out.
	* @param Name - Give the ability a name
	* @param NewDescription - Give a description of what the ability should do
	* @param NewTargetType - Who does the ability target?
	* @param NewPower - How strong is the ability's attack or heal?
	* @param NewAccuracy - How accurate is the ability?
	* @param NewMaxCooldown - Does the ability have a cooldown?
	* @param NewCooldown - What is the ability's current cooldown?
	* @param NewStatusType - Does the ability have a status effect?
	* @param StatusPower - How strong is the status effect in percentage?
	*/
	FAbilityStruct(FString Name, FString NewDescription, EMoveTypeEnum NewMoveType, ETargetTypeEnum NewTargetType, float NewPower, float NewAccuracy,int NewMaxCooldown, int NewCooldown, 
		EStatusTypeEnum NewStatusType, float NewStatusPower)
	{
		AbilityName = Name;
		Description = NewDescription;
		TargetType = NewTargetType;
		Power = NewPower;
		Accuracy = NewAccuracy;
		MaxCooldown = NewMaxCooldown;
		Cooldown = NewCooldown;
		StatusType = NewStatusType;
		StatusPower = NewStatusPower;
	}
};

//USTRUCT(BlueprintType)
//struct FStatusStruct : public FTableRowBase
//{
//	GENERATED_USTRUCT_BODY()
//
//public:
//
//	
//};
