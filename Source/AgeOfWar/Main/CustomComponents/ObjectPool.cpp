// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool.h"


// Sets default values for this component's properties
UObjectPool::UObjectPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UObjectPool::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UObjectPool::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UObjectPool::AcquireFromPool(UObject* Instigator)
{
	bIsInUse = true;
	GetOwner()->SetActorHiddenInGame(false);
	GetOwner()->SetActorEnableCollision(true);
	GetOwner()->SetActorTickEnabled(true);
	OnOwnerPooled.Broadcast(bIsInUse, Instigator);
	return true;
}

bool UObjectPool::ReleaseIntoPool(UObject* Instigator)
{
	bIsInUse = false;
	GetOwner()->SetActorHiddenInGame(true);
	GetOwner()->SetActorEnableCollision(false);
	GetOwner()->SetActorTickEnabled(false);
	OnOwnerPooled.Broadcast(bIsInUse, Instigator);
	return false;
}

