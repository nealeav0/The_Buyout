// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "Ability.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterActionStateEnum : uint8  {
	IDLE UMETA(DisplayName = "Idling"), 
	MOVE UMETA(DisplayName = "Moving")
};

USTRUCT(BlueprintType)
struct FPlayerStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityStruct> PlayerAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Evasion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Accuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EXP;

	// AtkBuf (multiplier added to player attack)
	// > 1 benefits player
	// < 1 benefits enemy  
	UPROPERTY()
	float AttackBuff;

	// DefBuf (multiplier added to player's received attacks)
	// < 1 benefits player
	// > 1 benefits enemy 
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

	FPlayerStruct()
	{
		Name = FString("Player");
		Level = 1;
	}
};

UCLASS()
class CS354P_F_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(EditAnywhere)
	FPlayerStruct PlayerStats = FPlayerStruct();

	// Temporary
	/*UPROPERTY(EditAnywhere)
	FEnemyStruct EnemyStats = FEnemyStruct();*/

	virtual void Landed(const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void MoveEvent(FVector2D Movement);

	UFUNCTION(BlueprintCallable)
	void MoveCameraEvent(FVector2D Movement);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    void ReadyForBattle();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main Character", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* Hitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) ECharacterActionStateEnum CharacterActionState;

	UFUNCTION(BlueprintCallable) 
	bool CanPerformAction(ECharacterActionStateEnum UpdatedAction);

	UFUNCTION(BlueprintCallable) 
	void UpdateActionState(ECharacterActionStateEnum NewAction);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void AnimateAttack();

    void ResetPosition();

	UFUNCTION(BlueprintCallable)
	void Escape();

	UFUNCTION(BlueprintCallable)
	FPlayerStruct GetPlayerStruct();

	UFUNCTION() 
	void OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY() FVector SpawnLocation;

	FTimerHandle TransitionTimer;

	void LoadBattle();

	void ResetCamera();

	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
