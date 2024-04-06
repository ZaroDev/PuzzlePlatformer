// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"

#include "Components/Button.h"


bool UInGameMenu::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	
	ContinueBtn->OnClicked.AddDynamic(this, &UInGameMenu::OnContinueClick);
	QuitBtn->OnClicked.AddDynamic(this, &UInGameMenu::OnQuitClick);
	
	return true;
}

void UInGameMenu::OnContinueClick()
{
	if(MenuInterface)
	{
		MenuInterface->UnPause();
	}
}

void UInGameMenu::OnQuitClick()
{
	if(MenuInterface)
	{
		MenuInterface->LeaveServer();
	}
}
