// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/DataTable.h"
#include "Ability.h"
#include "EntityBase.h"
#include "EnemyBase.generated.h"


UCLASS()
class CS354P_F_API AEnemyBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBase();

	UPROPERTY(EditAnywhere)
	FEntityStruct EnemyStats = FEntityStruct();

	UPROPERTY(EditAnywhere)
	TArray<FEntityStruct> Enemies = { EnemyStats };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	class USphereComponent* HitBox;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FORCEINLINE class USphereComponent* GetTriggerComponent() const { return HitBox; }

	UPROPERTY() FVector SpawnLocation;
	
	FTimerHandle TransitionTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void ResetPosition();

    virtual void Attack();

	virtual void Die();

	UFUNCTION()
	FEntityStruct GetEntityStruct();
    
	UFUNCTION()
	void SetEntityStructLocation(FVector Location);
};
