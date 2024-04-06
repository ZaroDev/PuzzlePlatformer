// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEMENUSYSTEM_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup();
	void Shutdown();
	void SetMenuInterface(IMenuInterface* Interface);

protected:
	IMenuInterface* MenuInterface = nullptr;
};
