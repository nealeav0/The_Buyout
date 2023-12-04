// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "EntityBase.h"
#include "Ability.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterActionStateEnum : uint8  {
	IDLE UMETA(DisplayName = "Idling"), 
	MOVE UMETA(DisplayName = "Moving")
};

UCLASS()
class CS354P_F_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(EditAnywhere)
	TArray<FEntityStruct> Players;

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
	FEntityStruct GetEntityStruct();

	UFUNCTION() 
	void OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY() FVector SpawnLocation;

	FTimerHandle TransitionTimer;

	void LoadBattle();

	void ResetCamera();

	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
