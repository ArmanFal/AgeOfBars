// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "UnitPoolHandler.h"

void UUnitPoolHandler::BeginUnitPoolInitialization(int PoolSize, TSubclassOf<AUnit> PoolClass)
{
	UnitsToPool = PoolSize;
	PooledUnitClass = PoolClass;
	if (!PoolClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pool class not found using Native Unit Class"));
		PooledUnitClass = AUnit::StaticClass();
	}
	InitializeUnitPool();
}

void UUnitPoolHandler::InitializeUnitPool()
{
	for (int i = 0; i < UnitsToPool; i++)
	{
		AUnit* Unit = GetWorld()->SpawnActor<AUnit>( PooledUnitClass, 
			FVector::ZeroVector,
			FRotator::ZeroRotator);
		
		if (!Unit)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn one or more Units to pool at UUnitPoolHandler Subsystem"));
			break;
		} 
		UObjectPool* ObjectPool = Unit->FindComponentByClass<UObjectPool>();
		if (!ObjectPool)
		{
			UE_LOG(LogTemp, Error, TEXT("No UObjectPool found When Spawning Units at UUnitPoolHandler Subsystem"));
			break;
		}
		ObjectPool->ReleaseIntoPool(this);
		// Unit->AssignLaneManager(this);
		// AddNewUnitToIndex(i, Unit);
		PooledUnits.Insert(Unit, 0);
		AllUnits.Insert(Unit, 0);
	}
	OnFinishedUnitPoolInitialization.Broadcast();
}

bool UUnitPoolHandler::RequestReleaseFromPool_Implementation(FGameplayTag ReleaseObjectGameplayTag, UObject* RequestedBy, UObject*& ReleasedObject)
{
	if (PooledUnits.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("The Pool Is Empty"));
		return false;
	}
	AUnit* Unit = PooledUnits.Pop(EAllowShrinking::No).Get();
	if (!Unit)
	{
		return false;
	}
	if (Unit->GetObjectPool()->AcquireFromPool(RequestedBy))
	{
		SpawnedUnits.Insert(Unit, 0);
		ReleasedObject = Unit;
		return true;
	}
	
	// Send it back to the pooled units if acquire fails
	PooledUnits.AddUnique(Unit);
	return false;
}

bool UUnitPoolHandler::RequestReturnToPool_Implementation(UObject* RequestingObject, UObject* ReturnedObject)
{
	if (!IsValid(ReturnedObject))
	{
		return false;
	}
	
	AUnit* Unit = Cast<AUnit>(ReturnedObject);
	
	if (!Unit)
	{
		return false;
	}
	
	Unit->GetObjectPool()->ReleaseIntoPool(ReturnedObject);
	SpawnedUnits.Remove(Unit);
	PooledUnits.AddUnique(Unit);
	return true;
}

