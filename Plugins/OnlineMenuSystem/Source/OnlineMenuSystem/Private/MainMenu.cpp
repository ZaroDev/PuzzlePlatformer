// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "ServerListItem.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UServerListItem> ServerListItemBPClass(TEXT("/OnlineMenuSystem/WBP_ServerListItem"));
	if (!ensure(ServerListItemBPClass.Class != nullptr))
	{
		return;
	}

	ServerListItemClass = ServerListItemBPClass.Class;
}

bool UMainMenu::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	
	HostBtn->OnClicked.AddDynamic(this, &UMainMenu::OnHostClick);
	JoinBtn->OnClicked.AddDynamic(this, &UMainMenu::OnJoinClick);
	BackBtn->OnClicked.AddDynamic(this, &UMainMenu::OnBackClick);
	JoinGameBtn->OnClicked.AddDynamic(this, &UMainMenu::UMainMenu::OnJoinGameClick);
	QuitBtn->OnClicked.AddDynamic(this, &UMainMenu::OnQuitClick);
	HostGameBtn->OnClicked.AddDynamic(this, &UMainMenu::OnHostGameClick);
	BackHostBtn->OnClicked.AddDynamic(this, &UMainMenu::OnBackClick);
	
	return true;
}

void UMainMenu::UpdateChildren()
{
	for(int32 i = 0; i < ServerScrollBox->GetChildrenCount(); i++)
	{
		const auto Item = Cast<UServerListItem>(ServerScrollBox->GetChildAt(i));
		if(!Item)
		{
			continue;
		}
		Item->IsSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
	}
}

void UMainMenu::OnHostClick()
{
	if(!ensure(MenuSwitcher != nullptr))
	{
		return;
	}

	if(!ensure(HostMenu != nullptr))
	{
		return;
	}
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::OnJoinClick()
{
	if(!ensure(MenuSwitcher != nullptr))
	{
		return;
	}

	if(!ensure(JoinMenu != nullptr))
	{
		return;
	}
	
	MenuSwitcher->SetActiveWidget(JoinMenu);
	MenuInterface->RefreshServerList();
}

void UMainMenu::SetServerList(TArray<FServerData> ServersData)
{
	if(!ensure(ServerScrollBox != nullptr))
	{
		return;
	}
	
	ServerScrollBox->ClearChildren();
	uint32 Index = 0;
	for(const auto& ServerData : ServersData)
	{
		UServerListItem* ListItem = CreateWidget<UServerListItem>(this, ServerListItemClass);
		if(!ensure(ListItem != nullptr))
		{
			continue;
		}

		ListItem->ServerName->SetText(FText::FromString(ServerData.Name));
		ListItem->HostName->SetText(FText::FromString(ServerData.HostUserName));
		ListItem->PlayerCount->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers)));
		ListItem->Setup(this, Index);
		
		ServerScrollBox->AddChild(ListItem);

		++Index;
	}
}

void UMainMenu::SetSelectedIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::OnJoinGameClick()
{
	if(SelectedIndex.IsSet() && MenuInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
}

void UMainMenu::OnBackClick()
{
	if(!ensure(MenuSwitcher != nullptr))
	{
		return;
	}

	if(!ensure(MainMenu != nullptr))
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OnQuitClick()
{
	if(MenuInterface)
	{
		MenuInterface->Quit();
	}
}

void UMainMenu::OnHostGameClick()
{
	if(MenuInterface && ServerNameBox)
	{
		MenuInterface->Host(ServerNameBox->GetText().ToString());
	}
}
