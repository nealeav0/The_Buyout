// Copyright Epic Games, Inc. All Rights Reserved.


#include "CS354p_FGameModeBase.h"
#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"

ACS354p_FGameModeBase::ACS354p_FGameModeBase()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_MainCharacter"));
	if (PlayerPawnBPClass.Class)
		DefaultPawnClass = PlayerPawnBPClass.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/BP_MainPlayerController"));
	if (PlayerControllerBPClass.Class)
		PlayerControllerClass = PlayerControllerBPClass.Class;

	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance) // temp
		GameInstance->SpawnEnemies();
}