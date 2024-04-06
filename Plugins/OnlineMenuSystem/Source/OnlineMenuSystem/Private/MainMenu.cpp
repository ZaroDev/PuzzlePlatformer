// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


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
	
	
	return true;
}

void UMainMenu::OnHostClick()
{
	if(MenuInterface)
	{
		MenuInterface->Host();
	}
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
}

void UMainMenu::OnJoinGameClick()
{
	if(!ensure(MenuInterface != nullptr))
	{
		return;
	}
	if(!ensure(IPAddressBox != nullptr))
	{
		return;
	}

	MenuInterface->Join(IPAddressBox->GetText().ToString());
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
