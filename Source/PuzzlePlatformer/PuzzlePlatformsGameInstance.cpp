// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "InGameMenu.h"
#include "MainMenu.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "OnlineSubsystem.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance()
{
	const ConstructorHelpers::FClassFinder<UMainMenu> MenuBPClass(TEXT("/OnlineMenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr))
	{
		return;
	}

	MenuClass = MenuBPClass.Class;

	const ConstructorHelpers::FClassFinder<UInGameMenu> InGameMenuBPClass(TEXT("/OnlineMenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr))
	{
		return;
	}

	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	Engine = GetEngine();
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (!SessionInterface.IsValid())
		{
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Found session interface"));
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
			this, &UPuzzlePlatformsGameInstance::OnSessionCreateComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
			this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
			this, &UPuzzlePlatformsGameInstance::OnSessionFindComplete); 
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(
			this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
	}
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr))
	{
		return;
	}

	MainMenu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(MainMenu))
	{
		return;
	}
	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr))
	{
		return;
	}

	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	if (!ensure(InGameMenu))
	{
		return;
	}
	InGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host(FString InServerName)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	if (auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession))
	{
		SessionInterface->DestroySession(NAME_GameSession);
		return;
	}
	this->ServerName = InServerName;
	
	CreateSession();
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if(MainMenu)
	{
		MainMenu->Shutdown();
	}
	if(!SessionInterface.IsValid())
	{
		return;
	}
	if(!SessionSearch.IsValid())
	{
		return;
	}

	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformsGameInstance::Pause()
{
	if (!InGameMenu)
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

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if(SessionSearch.IsValid())
	{
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionSearch->MaxSearchResults = 100;
		
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("Finding sessions"));
	}
}

void UPuzzlePlatformsGameInstance::StartSession()
{
	if(SessionInterface.IsUnique())
	{
		SessionInterface->StartSession(NAME_GameSession);
	}
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionNAme, bool Success)
{
	if (!Success)
	{
		return;
	}
	CreateSession();
}

void UPuzzlePlatformsGameInstance::OnSessionCreateComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to create session %s"), *SessionName.ToString());
		return;
	}
	if (MainMenu)
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

	World->ServerTravel("/Game/Maps/Lobby?listen");
}

void UPuzzlePlatformsGameInstance::OnSessionFindComplete(bool Success)
{
	if(!Success || !SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Error trying to find sessions"));
		return;
	}

	TArray<FServerData> ServersData;
	for(auto& SearchResult : SessionSearch->SearchResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session found named %s"), *SearchResult.GetSessionIdStr());

		FServerData Data;
		SearchResult.Session.SessionSettings.Get(TEXT("ServerName"), Data.Name);
		Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
		Data.HostUserName = SearchResult.Session.OwningUserName;
		
		ServersData.Add(Data);
	}

	MainMenu->SetServerList(ServersData);

	if(SessionSearch->SearchResults.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No sessions found"));
	}

	UE_LOG(LogTemp, Warning, TEXT("End find session"));
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(!SessionInterface.IsValid())
	{
		return;
	}
	FString Address;
	if(!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to resolve connect string"));
		return;
	}
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!PlayerController)
	{
		return;
	}
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set(TEXT("ServerName"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
}
