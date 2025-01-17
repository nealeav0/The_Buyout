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
	/*MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("Hitbox"));
	HitBox->SetupAttachment(MeshComponent);*/
	//HitBox->SetRelativeLocation(FVector(0, 0, 30.f));
	HitBox->InitSphereRadius(200);

    /*TriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerComponent"));
	TriggerComponent->SetupAttachment(RootComponent);
	TriggerComponent->InitSphereRadius(200.f);
	TriggerComponent->SetMobility(EComponentMobility::Movable);*/

	SpawnLocation = GetActorLocation();
}

void ACommonEnemy::BeginPlay()
{
	Super::BeginPlay();
	Enemies[0].EnemyType = EEnemyType::COMMON;
	/*Enemies[1].EnemyType = EEnemyType::COMMON;
	Enemies[2].EnemyType = EEnemyType::EVASIVE;*/
	// UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	// if (GameInstance)
	// {
	// 	// Set the common enemy's stats based on level
	// 	if (GameInstance->EnemyBaseDataTable)
	// 	{
	// 		FEntityStruct* CommonBase = GameInstance->EnemyBaseDataTable->FindRow<FEntityStruct>(FName(TEXT("common")), FString(TEXT("Getting Common Stats")));

	// 		if (CommonBase)
	// 		{
	// 			if (Enemies[0].Level == 0)
	// 			{
	// 				Enemies[0].Level = 1;
	// 			}
	// 			Enemies[0].Name = (*CommonBase).Name;
	// 			Enemies[0].EntityType = (*CommonBase).EntityType;
	// 			Enemies[0].MaxHealth = FMath::Floor((*CommonBase).MaxHealth * FMath::Pow(1.191, EnemyStats.Level));
	// 			Enemies[0].Health = EnemyStats.MaxHealth;
	// 			Enemies[0].Attack = FMath::Floor((*CommonBase).Attack * FMath::Pow(1.29, EnemyStats.Level));
	// 			Enemies[0].MagicAttack = FMath::Floor((*CommonBase).MagicAttack * FMath::Pow(1.29, EnemyStats.Level));
	// 			Enemies[0].Defense = FMath::Floor((*CommonBase).Defense + (*CommonBase).Defense * (EnemyStats.Level / 25));
	// 			Enemies[0].MagicDefense = FMath::Floor((*CommonBase).MagicDefense + (*CommonBase).Defense * (EnemyStats.Level / 25));
	// 			Enemies[0].Accuracy = FMath::Floor((*CommonBase).Accuracy * 0.84  * FMath::Pow(1.0844, EnemyStats.Level) * 2);
	// 			Enemies[0].Evasion = FMath::Floor((*CommonBase).Evasion * 0.84  * FMath::Pow(1.0844, EnemyStats.Level) * 2);
	// 			Enemies[0].EXP = FMath::Floor((*CommonBase).EXP * FMath::Pow(1.2, EnemyStats.Level));
	// 			Enemies[0].AbilityPoints = FMath::Floor((*CommonBase).AbilityPoints * FMath::Pow(1.25, EnemyStats.Level));
	// 			Enemies[0].ElementalResistances = (*CommonBase).ElementalResistances;
	// 			/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Common EXP: %d"), EnemyStats.EXP));
	// 			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Common Base EXP: %d"), (*CommonBase).EXP));
	// 			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Common AP: %d"), EnemyStats.AbilityPoints));
	// 			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Common Base AP: %d"), (*CommonBase).AbilityPoints));*/
	// 		}	
	// 	}
	// }
	
	// if we're in the overworld, let's loop some roaming behaviors
	if (GetWorld()->GetAuthGameMode() && !GetWorld()->GetAuthGameMode()->IsA(ABattleGameModeBase::StaticClass()))
    {
		itercounter = 0;
		CurrentDirection = Movements[0];
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
	GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Magenta, FString::Printf(TEXT("Common Died.")));
	// maybe we can add some animation 
	SpawnLocation = GetActorLocation();
	SetActorLocation(SpawnLocation + FVector(0.f, 0.f, 25.f));
	SetActorScale3D(GetActorScale3D() * 0.5);
	SpawnLocation -= FVector(0.f, 0.f, 25.f);
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &ACommonEnemy::ResetPosition, 0.1f, false);
	// GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &ACommonEnemy::DeleteSelf, 0.5f, false);
}

void ACommonEnemy::DeleteSelf() {
	this->Destroy();
}