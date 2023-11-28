// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameModeBase.h"

AMainMenuGameModeBase::AMainMenuGameModeBase()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_MainCharacter"));
	if (PlayerPawnBPClass.Class)
		DefaultPawnClass = PlayerPawnBPClass.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/BP_MainPlayerController"));
	if (PlayerControllerBPClass.Class)
		PlayerControllerClass = PlayerControllerBPClass.Class;
}
