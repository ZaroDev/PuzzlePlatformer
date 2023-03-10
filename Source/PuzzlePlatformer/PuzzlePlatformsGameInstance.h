// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API UPuzzlePlatformsGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPuzzlePlatformsGameInstance();

	virtual void Init() override;

	UFUNCTION(exec)
	void Host();

	UFUNCTION(exec)
	void Join(const FString& address);
private:

	class UEngine* Engine;
};
