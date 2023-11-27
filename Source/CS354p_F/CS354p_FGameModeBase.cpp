// Copyright Epic Games, Inc. All Rights Reserved.


#include "CS354p_FGameModeBase.h"

ACS354p_FGameModeBase::ACS354p_FGameModeBase()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_MainCharacter"));
	if (PlayerPawnBPClass.Class)
		DefaultPawnClass = PlayerPawnBPClass.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/BP_MainPlayerController"));
	if (PlayerControllerBPClass.Class)
		PlayerControllerClass = PlayerControllerBPClass.Class;

}