// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"

#include "MainMenu.generated.h"

/**
 * 
 */

class UButton;
class UWidget;

UCLASS()
class ONLINEMENUSYSTEM_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
private:
	UFUNCTION()
	void OnHostClick();

	UFUNCTION()
	void OnJoinClick();

	UFUNCTION()
	void OnJoinGameClick();

	UFUNCTION()
	void OnBackClick();

	UFUNCTION()
	void OnQuitClick();
	
	UPROPERTY(meta = (BindWidget))
	UButton* HostBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinGameBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu = nullptr;

	

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IPAddressBox = nullptr;
};
