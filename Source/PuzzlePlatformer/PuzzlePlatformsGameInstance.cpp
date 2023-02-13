// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("Constructor"));
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init"));

	Engine = GetEngine();
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (!ensure(Engine != nullptr))
		return;
	Engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, TEXT("Hosting game..."));

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr))
		return;

	world->ServerTravel("/Game/Maps/Gameplay?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& address)
{
	if (Engine)
		Engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Joining game at %s ..."), *address));

	APlayerController* playerController = GetFirstLocalPlayerController();

	if (!ensure(playerController != nullptr))
		return;

	playerController->ClientTravel(address, ETravelType::TRAVEL_Absolute);
}
