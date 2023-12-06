// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CommonEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CS354P_F_API ACommonEnemy : public AEnemyBase
{
	GENERATED_BODY()

public: 
	// Sets default values for this actor's properties
	ACommonEnemy();

	FORCEINLINE class USphereComponent* GetTriggerComponent() const { return TriggerComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* TriggerComponent;

	UPROPERTY() FVector SpawnLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* CommonEnemyMovementComponent;

	FTimerHandle TransitionTimer;

	FVector CurrentDirection;
	int32 itercounter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() 
	void Roam();

	UFUNCTION() 
	void ResetPosition();
	
	UFUNCTION() 
	virtual void Attack() override;

	UFUNCTION() 
	virtual void Die() override;

	UFUNCTION() 
	void DeleteSelf();
};
