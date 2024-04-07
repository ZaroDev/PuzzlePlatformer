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

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

UCLASS()
class ONLINEMENUSYSTEM_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServersData);
	void SetSelectedIndex(uint32 Index);
protected:
	virtual bool Initialize() override;
	
private:
	void UpdateChildren();

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

	UFUNCTION()
	void OnHostGameClick();
	
	UPROPERTY(meta = (BindWidget))
	UButton* HostBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* BackHostBtn = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UButton* HostGameBtn = nullptr;

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
	UWidget* HostMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerNameBox = nullptr;

	TSubclassOf<UUserWidget> ServerListItemClass;

	TOptional<uint32> SelectedIndex;
};
