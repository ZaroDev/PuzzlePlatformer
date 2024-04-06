// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "InGameMenu.h"
#include "MainMenu.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnlineSubsystem.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance()
{
	const ConstructorHelpers::FClassFinder<UMainMenu> MenuBPClass(TEXT("/OnlineMenuSystem/WBP_MainMenu"));
	if(!ensure(MenuBPClass.Class != nullptr))
	{
		return;
	}

	MenuClass = MenuBPClass.Class;

	const ConstructorHelpers::FClassFinder<UInGameMenu> InGameMenuBPClass(TEXT("/OnlineMenuSystem/WBP_InGameMenu"));
	if(!ensure(InGameMenuBPClass.Class != nullptr))
	{
		return;
	}

	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	Engine = GetEngine();

	if(const IOnlineSubsystem* OnlineSubsystem =  IOnlineSubsystem::Get())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session interface"));
		}
	}
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if(!ensure(MenuClass != nullptr))
	{
		return;
	}
	
	MainMenu =  CreateWidget<UMainMenu>(this, MenuClass);
	if(!ensure(MainMenu))
	{
		return;
	}
	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::InGameLoadMenu()
{
	if(!ensure(InGameMenuClass != nullptr))
	{
		return;
	}
	
	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	if(!ensure(InGameMenu))
	{
		return;
	}
	InGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
	if(MainMenu)
	{
		MainMenu->Shutdown();
	}
	
	if (!ensure(Engine != nullptr))
	{
		return;
	}
	
	Engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, TEXT("Hosting game..."));

	UWorld* World = GetWorld();

	if (!ensure(World != nullptr))
	{
		return;
	}

	World->ServerTravel("/Game/Maps/Gameplay?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	if (Engine)
	{
		Engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Joining game at %s ..."), *Address));
	}
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!ensure(PlayerController != nullptr))
	{
		return;
	}

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
void UPuzzlePlatformsGameInstance::Pause()
{
	if(!InGameMenu)
	{
		InGameLoadMenu();
	}
	InGameMenu->Setup();
}

void UPuzzlePlatformsGameInstance::UnPause()
{
	InGameMenu->Shutdown();
}

void UPuzzlePlatformsGameInstance::LeaveServer()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!ensure(PlayerController != nullptr))
	{
		return;
	}

	PlayerController->ClientTravel("/OnlineMenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::Quit()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!ensure(PlayerController != nullptr))
	{
		return;
	}
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
