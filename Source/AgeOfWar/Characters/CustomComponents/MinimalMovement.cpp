// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimalMovement.h"


// Sets default values for this component's properties
UMinimalMovement::UMinimalMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

}

void UMinimalMovement::BeginPlay()
{
	Super::BeginPlay();
	
	// Getting a validated OwnerActor we can use as reference later
	OwnerActor = GetOwner();
	check(OwnerActor);
	
	if (bShouldBeginMovementOnBeginPlay)
	{
		BeginMovement();
	}
}

void UMinimalMovement::BeginMovement()
{
	if (!EnsureOwnerExists())
	{
		return;
	}
	SetComponentTickEnabled(true);
}

void UMinimalMovement::StopMovement()
{
	SetComponentTickEnabled(false);
}

// Called every frame
void UMinimalMovement::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!EnsureOwnerExists())
	{
		SetComponentTickEnabled(false);
		return;
	}
	MoveUnit(bShouldOverrideUnitDirection? OverrideDirection:
		OwnerActor->GetActorForwardVector(),
		DeltaTime);
}

bool UMinimalMovement::EnsureOwnerExists() const
{
	// Usually OwnerActor will always be there based on Unreal's architecture but we include this for safety anyway
	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Has No Valid OwnerActor"), *GetNameSafe(this));
		return false;
	}
	return true;
}

void UMinimalMovement::MoveUnit(FVector Direction, float deltaTime)
{
	// Basic movement for one direction based on acceleration
	Velocity += Direction.GetSafeNormal() * Acceleration * deltaTime;
	
	Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);
	
	OwnerActor->AddActorWorldOffset(Velocity * deltaTime, true);
}

void UMinimalMovement::UpdateForwardDetection()
{
	
}
