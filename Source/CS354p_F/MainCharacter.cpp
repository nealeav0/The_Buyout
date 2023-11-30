// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "MainGameInstance.h"
#include "BattleGameModeBase.h"
#include "BattleCamera.h"
#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"

ABattleCamera* BattleCam;
AMainPlayerController* MainController;
bool bBattleReady;

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	CameraBoom->SetRelativeRotation(FRotator(-5.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	Hitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hitbox"));
	Hitbox->SetupAttachment(RootComponent);
	Hitbox->InitCapsuleSize(55.f, 100.f);
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	SpawnLocation = GetActorLocation();

	bBattleReady = false;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// At this point, we should already have the base stats loaded in.
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		// let's adjust the camera to signify that we're in battle mode
		ABattleGameModeBase* BattleMode = Cast<ABattleGameModeBase>(GetWorld()->GetAuthGameMode());
		if (BattleMode) {

			// switches to separate camera actor entirely
			BattleCam = Cast<ABattleCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ABattleCamera::StaticClass()));
			MainController = Cast<AMainPlayerController>(GetController());
			MainController->SetViewTargetWithBlend(BattleCam, 0.5f, EViewTargetBlendFunction::VTBlend_Linear);
		} else {
			// let's go back to where we last were before the battle
			// make sure this is NOT the very beginning of the game
			if (GameInstance->BattleManager()->Rounds > 0) {
				PlayerStats = GameInstance->BattleManager()->GetPlayer();
				SetActorLocation(GameInstance->GetPlayerLastLocation());
				GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &AMainCharacter::ReadyForBattle, 3.f, false); // give player enough time to run away before retriggering battle if they had just escaped
			} else {
				ReadyForBattle();
			}
		}
		
		// Set the player's stats based on level
		if (GameInstance->PlayerBaseDataTable)
		{
			FPlayerStruct* PlayerBase = GameInstance->PlayerBaseDataTable->FindRow<FPlayerStruct>(FName(TEXT("warrior")), FString(TEXT("Getting Stats")));

			if (PlayerBase)
			{
				PlayerStats.MaxHealth = FMath::Floor((*PlayerBase).MaxHealth * FMath::Pow(1.15, PlayerStats.Level));
				PlayerStats.Health = PlayerStats.MaxHealth;
				PlayerStats.Attack = FMath::Floor((*PlayerBase).Attack * FMath::Pow(1.13, PlayerStats.Level));
				PlayerStats.Defense = FMath::Floor((*PlayerBase).Defense * FMath::Pow(1.11, PlayerStats.Level));
				PlayerStats.Accuracy = FMath::Floor((*PlayerBase).Accuracy * FMath::Pow(1.08, PlayerStats.Level) * 2);
				PlayerStats.Evasion = FMath::Floor((*PlayerBase).Evasion * FMath::Pow(1.08, PlayerStats.Level) * 2);
			}
		}

		// Set the player's abilities
		if (GameInstance->PlayerDataTable)
		{
			// Get all of the player abilities from data and place them into the PlayerAbilities array.
			TArray<FAbilityStruct*> AbilityData;
			GameInstance->PlayerDataTable->GetAllRows<FAbilityStruct>(TEXT("TEST"), AbilityData);
			for (FAbilityStruct* Ability : AbilityData)
			{
				PlayerStats.Abilities.Add(*Ability);
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, PlayerStats.PlayerAbilities.Last().AbilityName);
			}
		}
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Level: %d"), PlayerStats.Level));
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Max Health: %f"), PlayerStats.MaxHealth));
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Attack: %f"), PlayerStats.Attack));
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Defense: %f"), PlayerStats.Defense));
	}

	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlapBegin);
}

void AMainCharacter::ReadyForBattle() {
	bBattleReady = true;
}

void AMainCharacter::ResetCamera() {
	MainController->SetViewTargetWithBlend(this, 0.5f, EViewTargetBlendFunction::VTBlend_Linear);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanPerformAction(ECharacterActionStateEnum::IDLE))
		UpdateActionState(ECharacterActionStateEnum::IDLE);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainCharacter::AnimateAttack() {
	// animate attack (maybe use Timeline for a nice curve if we have time)
	SpawnLocation = GetActorLocation();
	SetActorLocation(SpawnLocation + FVector(0.f, 150.f, 0.f));
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &AMainCharacter::ResetPosition, 0.1f, false);
}

void AMainCharacter::ResetPosition() {
	SetActorLocation(SpawnLocation);
}

void AMainCharacter::Escape()
{
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());

	if (GameInstance)
	{	
		SpawnLocation = GetActorLocation();
		SetActorLocation(SpawnLocation + FVector(0.f, -150.f, 0.f));
		// commented since escaping always means leaving the game for now; if evade we will need this again
		// GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &AMainCharacter::ResetPosition, 0.1f, false);
		AMainCharacter* MainPlayer = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass()));
		MainPlayer->ResetCamera();
	}
}

FEntityStruct AMainCharacter::GetEntityStruct()
{
	return PlayerStats;
}

void AMainCharacter::OnOverlapBegin(UPrimitiveComponent* newComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor);
	if (enemy && bBattleReady)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ENEMY ENCOUNTERED! ENTERING BATTLE..."));

		UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			// set up player to respawn where they last were 
			PlayerStats.Location = GetActorLocation();
			GameInstance->SetPlayerLastLocation(PlayerStats.Location);
			// save all enemy positions in overworld so we know where to respawn them again 
			TSubclassOf<AEnemyBase> EnemyClass = AEnemyBase::StaticClass();
			TArray<AActor*> SpawnedEnemies;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyClass, SpawnedEnemies);
			TArray<FVector> AllLocations;
			for (AActor* CurrEnemy : SpawnedEnemies) {
				AllLocations.Add(CurrEnemy->GetActorLocation());
			}
			GameInstance->SaveEnemyLocations(AllLocations);
			// handle info for this specific enemy we've encountered
			enemy->SetEntityStructLocation(enemy->GetActorLocation());
			// set up battle manager
			GameInstance->BattleManager()->PrepareForBattle(GetEntityStruct(), enemy->GetEntityStruct());
			GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &AMainCharacter::LoadBattle, 0.5f, false);
		}
	}
}

void AMainCharacter::LoadBattle() {
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	GameInstance->BattleManager()->LoadBattle();
}

void AMainCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
	UpdateActionState(ECharacterActionStateEnum::IDLE);
}

void AMainCharacter::MoveEvent(FVector2D Movement)
{	
	if (CanPerformAction(ECharacterActionStateEnum::MOVE)) {
		AddMovementInput(GetActorForwardVector(), Movement.Y);
		AddMovementInput(GetActorRightVector(), Movement.X);
		UpdateActionState(ECharacterActionStateEnum::MOVE);
	}
}

void AMainCharacter::MoveCameraEvent(FVector2D Movement)
{
	AddControllerPitchInput(-Movement.Y);
	AddControllerYawInput(Movement.X);
}

bool AMainCharacter::CanPerformAction(ECharacterActionStateEnum UpdatedAction)
{
	switch (CharacterActionState) {
		case ECharacterActionStateEnum:: IDLE:
			// You can do anything after Idle
			return true;
			break;
		case ECharacterActionStateEnum:: MOVE:
			return true;
			break;
	}
	return false;
}

void AMainCharacter::UpdateActionState(ECharacterActionStateEnum NewAction)
{
	if (NewAction == ECharacterActionStateEnum::MOVE || NewAction == ECharacterActionStateEnum::IDLE) {
		if (GetVelocity().Size() <= 0.01f) {
			CharacterActionState = ECharacterActionStateEnum::IDLE;
		} else {
			CharacterActionState = ECharacterActionStateEnum::MOVE;
		}
	} else {
		CharacterActionState = NewAction;
	}
}