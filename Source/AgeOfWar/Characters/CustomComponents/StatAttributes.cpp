// Fill out your copyright notice in the Description page of Project Settings.


#include "StatAttributes.h"


// Sets default values for this component's properties
UStatAttributes::UStatAttributes()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatAttributes::InitializeAttributes()
{
	UpdatedAttributes = SpawningAttributes;
}
// Called when the game starts
void UStatAttributes::BeginPlay()
{
	Super::BeginPlay();
	InitializeAttributes();
}


// Called every frame
void UStatAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatAttributes::HandleDeath()
{
	OnDeath.Broadcast();
}

