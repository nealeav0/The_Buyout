// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonEnemy.h"
#include "MainGameInstance.h"
#include "BattleGameModeBase.h"

TArray<FVector> Movements = {
	FVector(1.f, 0.f, 0.f),
	FVector(1.f, 1.f, 0.f),
	FVector(0.f, 1.f, 0.f),
	FVector(0.f, -1.f, 0.f),
	FVector(-1.f, 0.f, 0.f),
	FVector(1.f, -1.f, 0.f),
	FVector(-1.f, -1.f, 0.f),
	FVector(-1.f, 1.f, 0.f)
};

ACommonEnemy::ACommonEnemy()
{
    TriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerComponent"));
	TriggerComponent->SetupAttachment(RootComponent);
	TriggerComponent->InitSphereRadius(200.f);

	SpawnLocation = GetActorLocation();
}

void ACommonEnemy::BeginPlay()
{
	Super::BeginPlay();
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		// Set the common enemy's stats based on level
		if (GameInstance->CommonBaseDataTable)
		{
			FEnemyStruct* CommonBase = GameInstance->CommonBaseDataTable->FindRow<FEnemyStruct>(FName(TEXT("0")), FString(TEXT("Getting Stats")));

			EnemyStats.MaxHealth = FMath::Floor((*CommonBase).MaxHealth * FMath::Pow(1.191, EnemyStats.Level));
			EnemyStats.Health = EnemyStats.MaxHealth;
			EnemyStats.Attack = FMath::Floor((*CommonBase).Attack * FMath::Pow(1.29, EnemyStats.Level));
			EnemyStats.Defense = FMath::Floor((*CommonBase).Defense + (*CommonBase).Defense * (EnemyStats.Level/25));
			EnemyStats.Accuracy = FMath::Floor((*CommonBase).Accuracy * FMath::Pow(1.0844, EnemyStats.Level) * 2);
			EnemyStats.Evasion = FMath::Floor((*CommonBase).Evasion * FMath::Pow(1.0844, EnemyStats.Level) * 2);
			EnemyStats.EXP = FMath::Floor((*CommonBase).EXP * FMath::Pow(1.2, EnemyStats.Level));
		}

		// Set the common enemy's abilities
		if (GameInstance->CommonDataTable)
		{
			// Get all of the common enemy's abilities from data and place them into the EnemyAbilities array.
			TArray<FAbilityStruct*> AbilityData;
			GameInstance->CommonDataTable->GetAllRows<FAbilityStruct>(TEXT("TEST"), AbilityData);
			for (FAbilityStruct* Ability : AbilityData)
			{
				EnemyStats.Abilities.Add(*Ability);
			}
		}
	}
	
	// if we're in the overworld, let's loop some roaming behaviors
	if (GetWorld()->GetAuthGameMode() && !GetWorld()->GetAuthGameMode()->IsA(ABattleGameModeBase::StaticClass()))
    {
		itercounter = 0;
		CurrentDirection = Movements[2];
        GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &ACommonEnemy::Roam, 0.05f, true);
    }
}

// Called every frame
void ACommonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// mock roaming
void ACommonEnemy::Roam()
{
	/* old implementation, kind of just goes in whatever direction
	if (itercounter % 50 == 0) 
		CurrentDirection = Movements[FMath::RandRange(0, Movements.Num() - 1)];
	
	FVector NewLocation = GetActorLocation() + (10 * CurrentDirection);
	SetActorLocation(NewLocation);
	// we should check if the new location is valid or not; if not, choose a different direction until it is valid
	itercounter++; */

	// let's just have it move side to side
	if (itercounter % 50 == 0) {
		CurrentDirection *= -1.f;
	}
	SetActorLocation(GetActorLocation() + (10 * CurrentDirection));
	itercounter++;
}

void ACommonEnemy::ResetPosition()
{
	SetActorLocation(SpawnLocation);
}

void ACommonEnemy::Attack()
{
	SpawnLocation = GetActorLocation();
	SetActorLocation(SpawnLocation + FVector(0.f, -150.f, 0.f));
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &ACommonEnemy::ResetPosition, 0.1f, false);
}

void ACommonEnemy::Die()
{	
	// maybe we can add some animation 
	SpawnLocation = GetActorLocation();
	SetActorLocation(SpawnLocation + FVector(0.f, 0.f, 25.f));
	SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	SpawnLocation -= FVector(0.f, 0.f, 25.f);
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &ACommonEnemy::ResetPosition, 0.1f, false);
	// GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &ACommonEnemy::DeleteSelf, 0.5f, false);
}

void ACommonEnemy::DeleteSelf() {
	this->Destroy();
}