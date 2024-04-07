// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerListItem.h"

#include "MainMenu.h"
#include "Components/Button.h"

void UServerListItem::Setup(UMainMenu* Owner, uint32 InIndex)
{
	SelectBtn->OnClicked.AddDynamic(this, &UServerListItem::OnSelectClick);
	this->Index = InIndex;
	this->Parent = Owner;
}

void UServerListItem::OnSelectClick()
{
	Parent->SetSelectedIndex(Index);
}
