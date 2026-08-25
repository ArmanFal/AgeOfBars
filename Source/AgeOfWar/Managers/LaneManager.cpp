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

AUnit* ALaneManager::SpawnUnitOfTypeOnLane()
{
	if (PooledUnits.IsEmpty())
	{
		return false;
	}
	TObjectPtr<AUnit> Unit = PooledUnits.Pop(false);
	
	if (Unit->GetObjectPool()->AcquireFromPool())
	{
		Unit->SetActorTransform(SpawnTransform);
		SpawnedUnits.Add(Unit);
		return true;
	}
	return false;
}

void ALaneManager::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < UnitsToPool; i++)
	{
		AUnit* Unit = GetWorld()->SpawnActor<AUnit>( AUnit::StaticClass(), 
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
		ObjectPool->ReleaseIntoPool();
		PooledUnits.Add(Unit);
	}
		
	SpawnTransform = BillboardSpawnLocation->GetComponentTransform();
}
