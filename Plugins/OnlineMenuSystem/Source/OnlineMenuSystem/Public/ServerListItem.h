// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerListItem.generated.h"

/**
 * 
 */
class UMainMenu;
class UTextBlock;

UCLASS()
class ONLINEMENUSYSTEM_API UServerListItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(UMainMenu* Owner, uint32 Index);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HostName = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerCount = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool IsSelected = false;
	
private:
	UFUNCTION()
	void OnSelectClick();
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SelectBtn = nullptr;

	UPROPERTY()
	UMainMenu* Parent = nullptr;
	
	uint32 Index = 0;
};
