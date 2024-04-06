// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"

#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
class UMenuWidget;

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
	virtual void Host() override;

	UFUNCTION(exec)
	virtual void Join(const FString& Address) override;

	UFUNCTION(BlueprintCallable)
	virtual void Pause() override;

	UFUNCTION()
	virtual void UnPause() override;

	UFUNCTION()
	virtual void LeaveServer() override;

	UFUNCTION()
	virtual void Quit() override;
private:

	class UEngine* Engine;

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	UMenuWidget* MainMenu = nullptr;
	UMenuWidget* InGameMenu = nullptr;
};
