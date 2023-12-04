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

	Players.Init(FEntityStruct(), 2); // I wish I could place the number of players in some global variable
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
		}
		else {
			// let's go back to where we last were before the battle
			// make sure this is NOT the very beginning of the game
			// Since the BattleManager does not empty its arrays until PrepareForBattle, we can use whatever is stored inside.
			if (!GameInstance->BattleManager()->Players.IsEmpty()) {
				for (int i = 0; i < Players.Num(); i++)
				{
					Players[i] = GameInstance->BattleManager()->Players[i];
				}
				SetActorLocation(GameInstance->GetPlayerLastLocation());
				GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &AMainCharacter::ReadyForBattle, 3.f, false); // give player enough time to run away before retriggering battle if they had just escaped
			}
			else {
				ReadyForBattle();
			}
		}
	}

	// Set the player's stats based on level
	if (GameInstance->PlayerBaseDataTable)
	{
		FEntityStruct* PlayerBase = GameInstance->PlayerBaseDataTable->FindRow<FEntityStruct>(FName(TEXT("warrior")), FString(TEXT("Getting Warrior Stats")));
		if (PlayerBase)
		{

			if (Players[0].Level == 0)
			{
				Players[0].Level = 1;
			}
			Players[0].Name = (*PlayerBase).Name;
			Players[0].EntityType = (*PlayerBase).EntityType;
			Players[0].MaxHealth = FMath::Floor((*PlayerBase).MaxHealth * FMath::Pow(1.15, Players[0].Level));
			if (GameInstance->BattleManager()->Players.IsEmpty())
			{
				Players[0].Health = Players[0].MaxHealth;
			}
			Players[0].EXPThreshold = FMath::Floor(50 * FMath::Pow(1.39, Players[0].Level));
			Players[0].Attack = FMath::Floor((*PlayerBase).Attack * FMath::Pow(1.13, Players[0].Level));
			Players[0].MagicAttack = FMath::Floor((*PlayerBase).MagicAttack * FMath::Pow(1.13, Players[0].Level));
			Players[0].Defense = FMath::Floor((*PlayerBase).Defense * FMath::Pow(1.11, Players[0].Level));
			Players[0].MagicDefense = FMath::Floor((*PlayerBase).MagicDefense * FMath::Pow(1.11, Players[0].Level));
			Players[0].Accuracy = FMath::Floor((*PlayerBase).Accuracy * FMath::Pow(1.08, Players[0].Level) * 2);
			Players[0].Evasion = FMath::Floor((*PlayerBase).Evasion * FMath::Pow(1.08, Players[0].Level) * 2);
			/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Player Max Health: %f"), Players[0].MaxHealth));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Player Health: %f"), Players[0].Health));*/
		}

		PlayerBase = GameInstance->PlayerBaseDataTable->FindRow<FEntityStruct>(FName(TEXT("mage")), FString(TEXT("Getting Mage Stats")));

		if (PlayerBase)
		{
			if (Players[1].Level == 0)
			{
				Players[1].Level = 1;
			}
			Players[1].Name = (*PlayerBase).Name;
			Players[1].EntityType = (*PlayerBase).EntityType;
			Players[1].MaxHealth = FMath::Floor((*PlayerBase).MaxHealth * FMath::Pow(1.15, Players[1].Level));
			if (GameInstance->BattleManager()->Players.IsEmpty())
			{
				Players[1].Health = Players[1].MaxHealth;
			}
			Players[1].EXPThreshold = FMath::Floor(50 * FMath::Pow(1.39, Players[1].Level));
			Players[1].Attack = FMath::Floor((*PlayerBase).Attack * FMath::Pow(1.13, Players[1].Level));
			Players[1].MagicAttack = FMath::Floor((*PlayerBase).MagicAttack * FMath::Pow(1.13, Players[1].Level));
			Players[1].Defense = FMath::Floor((*PlayerBase).Defense * FMath::Pow(1.11, Players[1].Level));
			Players[1].MagicDefense = FMath::Floor((*PlayerBase).Defense * FMath::Pow(1.11, Players[1].Level));
			Players[1].Accuracy = FMath::Floor((*PlayerBase).Accuracy * FMath::Pow(1.08, Players[1].Level) * 2);
			Players[1].Evasion = FMath::Floor((*PlayerBase).Evasion * FMath::Pow(1.08, Players[1].Level) * 2);
		}
	}


	GameInstance->AbilityManager()->InitializePlayerArray(Players);

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
	return Players[0];
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
			for (FEntityStruct& Player : Players)
			{
				Player.Location = GetActorLocation();
				GameInstance->AbilityManager()->InitializeAbilities(Player);
			}
			GameInstance->SetPlayerLastLocation(Players[0].Location);
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
			GameInstance->BattleManager()->PrepareForBattle(Players, enemy->GetEntityStruct());
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