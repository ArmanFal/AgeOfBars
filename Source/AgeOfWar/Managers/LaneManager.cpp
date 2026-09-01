// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "LaneManager.h"

#include "Characters/Unit.h"

ALaneManager::ALaneManager()
{
	BillboardActorIndicator = CreateDefaultSubobject<UBillboardComponent>(TEXT("LaneBillboard"));
	BillboardSpawnLocation = CreateDefaultSubobject<UBillboardComponent>(TEXT("LaneSpawnLocation"));
	
	SetRootComponent(BillboardActorIndicator);
	BillboardSpawnLocation->SetupAttachment(BillboardActorIndicator);
	
}

AUnit* ALaneManager::SpawnUnitOfTypeOnLane(UUnitData* UnitData)
{
	if (PooledUnits.IsEmpty())
	{
		return nullptr;
	}
	TObjectPtr<AUnit> Unit = PooledUnits.Pop(EAllowShrinking::No);
	if (!Unit)
	{
		return nullptr;
	}
	if (Unit->GetObjectPool()->AcquireFromPool(this))
	{
		// Make this into a delegate inside the UNIT
		if (Unit->ApplyDataToUnit(UnitData))
		{
			Unit->SetActorLocation(SpawnTransform.GetLocation());
			Unit->SetActorRotation(SpawnTransform.GetRotation());
			SpawnedUnits.Add(Unit);
			return Unit;
		}
	}
	return nullptr;
}

void ALaneManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (!PooledUnitClass)
	{
		return;
	}
	
	for (int i = 0; i < UnitsToPool; i++)
	{
		AUnit* Unit = GetWorld()->SpawnActor<AUnit>( PooledUnitClass, 
			FVector::ZeroVector,
			FRotator::ZeroRotator);
		
		if (!Unit)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn one or more Units to pool as LaneManager"));
			break;
		} 
		UObjectPool* ObjectPool = Unit->FindComponentByClass<UObjectPool>();
		if (!ObjectPool)
		{
			UE_LOG(LogTemp, Error, TEXT("No UObjectPool found When Spawning Units as LaneManager"));
			break;
		}
		ObjectPool->ReleaseIntoPool(this);
		PooledUnits.Add(Unit);
	}
		
	SpawnTransform = BillboardSpawnLocation->GetComponentTransform();
}

bool ALaneManager::RequestReturnToPool_Implementation(AUnit* RequestSource)
{
	if (!IsValid(RequestSource))
	{
		return false;
	}
	RequestSource->GetObjectPool()->ReleaseIntoPool(RequestSource);
	HandleUnitPool(RequestSource);
	return true;
}

void ALaneManager::HandleUnitPool(AUnit* PooledUnit)
{
	PooledUnits.Add(PooledUnit);
}

void ALaneManager::ReOrderSpawnedUnitsIndex(int FromIndex, int ToIndex, int Change)
{
	// 
}
