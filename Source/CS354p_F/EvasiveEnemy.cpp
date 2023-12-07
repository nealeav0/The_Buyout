// Fill out your copyright notice in the Description page of Project Settings.


#include "EvasiveEnemy.h"

AEvasiveEnemy::AEvasiveEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	/*MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("Hitbox"));
	HitBox->SetupAttachment(MeshComponent);
	HitBox->SetRelativeLocation(FVector(0, 0, 30.f));
	HitBox->InitSphereRadius(100);*/
}

void AEvasiveEnemy::BeginPlay()
{
	Super::BeginPlay();
	Enemies[0].EnemyType = EEnemyType::EVASIVE;
}

void AEvasiveEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEvasiveEnemy::Attack()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Evasive Attacks.")));
}

void AEvasiveEnemy::Die()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Evasive Died.")));
}
