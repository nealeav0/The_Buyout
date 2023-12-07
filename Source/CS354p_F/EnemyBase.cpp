// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/SphereComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("Hitbox"));
	HitBox->SetupAttachment(MeshComponent);
	HitBox->SetRelativeLocation(FVector(0, 0, 30.f));
	HitBox->InitSphereRadius(100);

	EnemyStats.Abilities.Add(FAbilityStruct());
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::ResetPosition()
{
	SetActorLocation(SpawnLocation);
}

void AEnemyBase::Attack()
{
	// maybe we can add some animation 
	SpawnLocation = GetActorLocation();
	SetActorLocation(SpawnLocation + FVector(0.f, -150.f, 0.f));
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &AEnemyBase::ResetPosition, 0.1f, false);
}

void AEnemyBase::Die()
{
	SpawnLocation = GetActorLocation();
	SetActorLocation(SpawnLocation + FVector(0.f, 0.f, 25.f));
	SetActorScale3D(GetActorScale3D() * 0.5);
	SpawnLocation -= FVector(0.f, 0.f, 25.f);
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &AEnemyBase::ResetPosition, 0.1f, false);
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("EnemyBase Died.")));
}

FEntityStruct AEnemyBase::GetEntityStruct()
{
	return EnemyStats;
}

void AEnemyBase::SetEntityStructLocation(FVector Location)
{
	EnemyStats.Location = Location;
}