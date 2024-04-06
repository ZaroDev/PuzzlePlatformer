// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::Setup()
{
	AddToViewport();

	const UWorld* World = GetWorld();
	if(!ensure(World != nullptr))
	{
		return;
	}
	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (!ensure(PlayerController != nullptr))
	{
		return;
	}
	FInputModeUIOnly InputMode{};
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(TakeWidget());
	
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::Shutdown()
{
	RemoveFromParent();
	
	const auto* World = GetWorld();
	if(!ensure(World != nullptr))
	{
		return;
	}
	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (!ensure(PlayerController != nullptr))
	{
		return;
	}
	const FInputModeGameOnly InputMode{};
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;
}

void UMenuWidget::SetMenuInterface(IMenuInterface* Interface)
{
	if(!ensure(Interface != nullptr))
	{
		return;
	}
	MenuInterface = Interface;
}
