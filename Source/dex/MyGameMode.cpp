// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "dex.h"
#include "MyGameMode.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "GenerateWorld.h"
#include "WorldCube.h"
#include "WorldSingleton.h"

AMyGameMode::AMyGameMode()
{
	WorldSingleton* Singleton = WorldSingleton::Instance();

	// use our custom PlayerController class
	PlayerControllerClass = AMyPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	/*
	for (auto& Elem : Singleton->WorldCubes)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString("Iterating through FMap: ") + Elem.Key);
	}
	*/
	
}