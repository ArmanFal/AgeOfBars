// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "LaneManager.h"

#include "Main/Unit.h"

ALaneManager::ALaneManager()
{
	BillboardActorIndicator = CreateDefaultSubobject<UBillboardComponent>(TEXT("LaneBillboard"));
	BillboardSpawnLocation = CreateDefaultSubobject<UBillboardComponent>(TEXT("LaneSpawnLocation"));
	
	SetRootComponent(BillboardActorIndicator);
	BillboardSpawnLocation->SetupAttachment(BillboardActorIndicator);
	OnNewUnitSpawned.AddDynamic(this, &ALaneManager::K2_OnNewUnitSpawned);
}

AUnit* ALaneManager::SpawnUnitOfTypeOnLane(UUnitData* UnitData)
{
	if (PooledUnits.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Maximum Unit Has Reached on this Lane"));
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
			AddNewUnitToIndex(0, Unit);
			return Unit;
		}
	}
	return nullptr;
}

void ALaneManager::BeginPlay()
{
	Super::BeginPlay();
	
	LanePowerCalculator = NewObject<ULanePowerCalculator>();
	
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
		Unit->AssignLaneManager(this);
		AddNewUnitToIndex(i, Unit);
		PooledUnits.Insert(Unit, 0);
	}
		
	BindDelegatesForLanePowerModification();
	SpawnTransform = BillboardSpawnLocation->GetComponentTransform();
}

bool ALaneManager::RequestReturnToPool_Implementation(UObject* RequestingObject)
{
	if (!IsValid(RequestingObject))
	{
		return false;
	}
	AUnit* Unit = Cast<AUnit>(RequestingObject);
	if (!Unit)
	{
		return false;
	}
	Unit->GetObjectPool()->ReleaseIntoPool(RequestingObject);
	HandleUnitPool(Unit);
	SpawnedUnits.Remove(Unit);
	return true;
}

void ALaneManager::HandleUnitPool(AUnit* PooledUnit)
{
	PooledUnits.Add(PooledUnit);
	OnNewUnitDespawned.Broadcast(PooledUnit);
}

void ALaneManager::AddNewUnitToIndex(const int Index, AUnit* NewUnit)
{
	if (NewUnit)
	{
		SpawnedUnits.Insert(NewUnit, Index);
		OnNewUnitSpawned.Broadcast(NewUnit);
	}
}

int ALaneManager::GetUnitIndex(AUnit* Unit) const
{
	return SpawnedUnits.Find(Unit);
}

void ALaneManager::BindDelegatesForLanePowerModification()
{
	OnNewUnitSpawned.AddDynamic(this, &ALaneManager::ModifyLanePower);
	OnNewUnitDespawned.AddDynamic(this, &ALaneManager::ModifyLanePower);
}

void ALaneManager::ModifyLanePower(AUnit* Source)
{
	if (!GetLanePowerCalculator())
	{
		UE_LOG(LogTemp, Warning, 
			TEXT("For %s a LanePower Calculator was not created, AI will not behave as intended"), *GetName());
	}
	bool bIsFriendly = Source->GetTeamAffiliationIndex() == TeamAffiliation;
	GetLanePowerCalculator()->ExecuteUnitWeightCalculation(Source, bIsFriendly);
}

