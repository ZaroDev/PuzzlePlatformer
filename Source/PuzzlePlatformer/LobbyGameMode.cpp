// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayerCount++;

	if(PlayerCount >= 2)
	{
		GetWorldTimerManager().SetTimer(StartGameHandle, this, &ALobbyGameMode::StartGame, 10.0f, false);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	PlayerCount--;
	Super::Logout(Exiting);
}

void ALobbyGameMode::StartGame()
{
	auto* GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if(!GameInstance)
	{
		return;
	}
	GameInstance->StartSession();

	
	UWorld* World = GetWorld();

	if (!ensure(World != nullptr))
	{
		return;
	}
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Gameplay?listen");
}
