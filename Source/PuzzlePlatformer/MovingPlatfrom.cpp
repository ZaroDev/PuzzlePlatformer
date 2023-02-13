// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatfrom.h"

AMovingPlatfrom::AMovingPlatfrom()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatfrom::BeginPlay()
{
	Super::BeginPlay();

	// Set the actor to sync between the server and the client
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatfrom::Tick(float DeltaTime)  
{
	Super::Tick(DeltaTime);
	

	if (ActiveTriggers > 0)
	{
		if (HasAuthority())
		{
			FVector location = GetActorLocation();
			float journeyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
			float journeyTraveled = (location - GlobalStartLocation).Size();

			if (journeyTraveled > journeyLength)
			{
				FVector swap = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = swap;
			}
			FVector direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
			location += direction * Speed * DeltaTime;
			SetActorLocation(location);
		}
	}
}

void AMovingPlatfrom::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatfrom::RemoveActiveTrigger()
{
	if(ActiveTriggers > 0)
		ActiveTriggers--;
}
