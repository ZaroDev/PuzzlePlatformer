// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */

class UButton;

UCLASS()
class ONLINEMENUSYSTEM_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;
	
private:
	UFUNCTION()
	void OnContinueClick();
	
	UFUNCTION()
	void OnQuitClick();

	UPROPERTY(meta = (BindWidget))
	UButton* ContinueBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitBtn = nullptr;
};
