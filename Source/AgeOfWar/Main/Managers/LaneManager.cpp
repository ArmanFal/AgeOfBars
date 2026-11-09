// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "LaneManager.h"

#include "Main/Unit.h"

ALaneManager::ALaneManager()
{
	BillboardActorIndicator = CreateDefaultSubobject<UBillboardComponent>(TEXT("LaneBillboard"));
	BillboardSpawnLocation = CreateDefaultSubobject<UBillboardComponent>(TEXT("LaneSpawnLocation"));
	
	SetRootComponent(BillboardActorIndicator);
	BillboardSpawnLocation->SetupAttachment(BillboardActorIndicator);
}

// AUnit* ALaneManager::SpawnUnitOfTypeOnLane(UUnitData* UnitData)
// {
// 	if (PooledUnits.IsEmpty())
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Maximum Unit Has Reached on this Lane"));
// 		return nullptr;
// 	}
// 	TObjectPtr<AUnit> Unit = PooledUnits.Pop(EAllowShrinking::No);
// 	if (!Unit)
// 	{
// 		return nullptr;
// 	}
// 	if (Unit->GetObjectPool()->AcquireFromPool(this))
// 	{
// 		// Make this into a delegate inside the UNIT
// 		if (Unit->ApplyDataToUnit(UnitData))
// 		{
// 			Unit->SetActorLocation(SpawnTransform.GetLocation());
// 			Unit->SetActorRotation(SpawnTransform.GetRotation());
// 			AddNewUnitToIndex(0, Unit);
// 			return Unit;
// 		}
// 	}
// 	return nullptr;
// }

void ALaneManager::BeginPlay()
{
	Super::BeginPlay();
	
	// LanePowerCalculator = NewObject<ULanePowerCalculator>();
		
	BindDelegatesForLanePowerModification();
	SpawnTransform = BillboardSpawnLocation->GetComponentTransform();
}

// bool ALaneManager::RequestReturnToPool_Implementation(UObject* RequestingObject, UObject* ReturnedObject)
// {
// 	if (!IsValid(RequestingObject))
// 	{
// 		return false;
// 	}
// 	
// 	AUnit* Unit = Cast<AUnit>(RequestingObject);
// 	
// 	if (!Unit)
// 	{
// 		return false;
// 	}
// 	
// 	Unit->GetObjectPool()->ReleaseIntoPool(RequestingObject);
// 	HandleUnitPool(Unit);
// 	SpawnedUnits.Remove(Unit);
// 	return true;
// }



bool ALaneManager::AddNewUnitToIndex(const int Index, AUnit* NewUnit, const bool SpawnFromLaneBegin)
{
	if (NewUnit)
	{
		SpawnedUnits.Insert(NewUnit, Index);
		NewUnit->AssignLaneManager(this);
		OnNewUnitSpawned.Broadcast(NewUnit, SpawnFromLaneBegin);
		return true;
	}
	return false;
}

int ALaneManager::GetUnitIndex(AUnit* Unit) const
{
	return SpawnedUnits.Find(Unit);
}

void ALaneManager::BindDelegatesForLanePowerModification()
{
	OnNewUnitSpawned.AddDynamic(this, &ALaneManager::ModifyLanePowerNewUnitSpawnedWrapper);
	OnNewUnitDespawned.AddDynamic(this, &ALaneManager::ModifyLanePower);
}

void ALaneManager::ModifyLanePowerNewUnitSpawnedWrapper(AUnit* NewUnit, const bool SpawnFromLaneBegin)
{
	ModifyLanePower(NewUnit);
}

void ALaneManager::ModifyLanePower(AUnit* Source)
{
	// if (!GetLanePowerCalculator())
	// {
	// 	UE_LOG(LogTemp, Warning, 
	// 		TEXT("For %s a LanePower Calculator was not created, AI will not behave as intended"), *GetName());
	// }
	// bool bIsFriendly = Source->GetTeamAffiliationIndex() == TeamAffiliation;
	// GetLanePowerCalculator()->ExecuteUnitWeightCalculation(Source, bIsFriendly);
}

bool ALaneManager::RemoveUnitFromLaneTracking(AUnit* Unit)
{
	if (SpawnedUnits.Find(Unit) != INDEX_NONE)
	{
		SpawnedUnits.Remove(Unit);
		return true;
	}
	return false;
}
