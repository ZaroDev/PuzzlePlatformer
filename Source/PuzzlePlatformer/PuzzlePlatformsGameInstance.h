// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class PUZZLEPLATFORMER_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UPuzzlePlatformsGameInstance();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();
	
	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();
	
	UFUNCTION(exec)
	virtual void Host(FString ServerName) override;

	UFUNCTION(exec)
	virtual void Join(uint32 Index) override;

	UFUNCTION(BlueprintCallable)
	virtual void Pause() override;

	UFUNCTION()
	virtual void UnPause() override;

	UFUNCTION()
	virtual void LeaveServer() override;

	UFUNCTION()
	virtual void Quit() override;

	UFUNCTION()
	virtual void RefreshServerList() override;

	void StartSession();
private:
	void OnDestroySessionComplete(FName SessionNAme, bool Success);
	void OnSessionCreateComplete(FName SessionName, bool Success);
	void OnSessionFindComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();
	
	class UEngine* Engine;

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* MainMenu = nullptr;
	class UInGameMenu* InGameMenu = nullptr;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	FString ServerName;
};
