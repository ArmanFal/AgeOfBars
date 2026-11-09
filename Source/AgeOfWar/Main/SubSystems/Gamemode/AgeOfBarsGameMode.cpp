// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "AgeOfBarsGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Main/Unit.h"
#include "Main/SubSystems/SubsystemLibrary.h"
#include "Main/SubSystems/UnitSubsystem.h"
#include "Main/SubSystems/PoolSingleton/UnitPoolHandler.h"

void AAgeOfBarsGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	MapLanes();
}

FVector AAgeOfBarsGameMode::RequestLocationOfLane_Implementation(int TeamIndex, EUnitLane Lane)
{
	if (auto* MapKeyValue = LaneMap.Find(TeamIndex))
	{
		if (TObjectPtr<ALaneManager>* SpecificLane = MapKeyValue->Find(Lane))
		{
			if (!SpecificLane->Get())
			{
				return FVector::ZeroVector;
			}
			return SpecificLane->Get()->GetActorLocation();
		}
	}
	return FVector::ZeroVector;
}

bool AAgeOfBarsGameMode::RequestSpawnUnit_Implementation(int ForTeam, FGameplayTag UnitToSpawn, EUnitLane LaneToSpawn, FString& FailureLog)
{
	auto* TeamLanes = FindAppropriateMapForTeamIndex(ForTeam);
	if (!TeamLanes)
	{
		FailureLog = "Did not find requested team";
		return false;
	}
	TObjectPtr<ALaneManager>* Lane = TeamLanes->Find(LaneToSpawn);
	if (!Lane)
	{
		FailureLog = "Did not find Requested Lane";
		return false;
	}
	
	UUnitPoolHandler* UnitPoolHandler = GetWorld()->GetSubsystem<UUnitPoolHandler>();
	if (!UnitPoolHandler)
	{
		FailureLog = "Did not find the UnitPoolHandler subsystem";
		return false;
	}
	UObject* PooledObject = nullptr;
	
	if (!IObjectPoolHandlerInterface::Execute_RequestReleaseFromPool(UnitPoolHandler, UnitToSpawn, this, PooledObject))
	{
		FailureLog = "Request To release from pool was denied. check UnitPoolHandler's implementation for the reason";
			return false;
	}
	AUnit* Unit = Cast<AUnit>(PooledObject);
		if (!Unit)
		{
			FailureLog = FString::Printf(TEXT("%s is not a unit"), *GetNameSafe(PooledObject));
			return false;
		}
	const UUnitSubsystem* USS = SubsystemsGetter::GetGameInstanceSubsystem<UUnitSubsystem>(this);
	const UUnitData* Ud = USS->GetUnitData(UnitToSpawn);
	
	if (!Ud)
	{
		FailureLog = "Did not find requested unit tag data";
		return false;
	}
	if (!Unit->ApplyDataToUnit(Ud))
	{
		FailureLog = "Couldn't Assign The Validated Data To The Unit.";
		return false;
	}
	
	if (Lane->Get()->AddNewUnitToIndex(0, Unit, true))
	{
		// FailureLog = FString::Printf(TEXT("Spawning on Lane : %s"), *Lane->Get()->GetName());
		return true;
	}
	FailureLog = "Couldn't Spawn Unit of type on lane";
	return false;
}

void AAgeOfBarsGameMode::MapLanes()
{
	TArray<AActor*> AllLanes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALaneManager::StaticClass(), AllLanes);
	
	for (AActor* Actor : AllLanes)
	{
		ALaneManager* LaneManager = Cast<ALaneManager>(Actor);

		if (!LaneManager)
		{
			continue;
		}

		const int TeamIndex = LaneManager->GetTeamAffiliation();
		const EUnitLane Lane = LaneManager->UnitLane;

		auto& TeamLanes = LaneMap.FindOrAdd(TeamIndex);
		
		if (TeamLanes.Contains(Lane))
		{
			UE_LOG(LogTemp, Warning, TEXT("Duplicate lane Manager on Team %s, and lane %s"), *FString::FromInt(TeamIndex), *UEnum::GetValueAsString(Lane) );
			continue;
		}
		TeamLanes.Add(Lane, LaneManager);
	}
}

bool AAgeOfBarsGameMode::RequestSendUnitToNewLane_Implementation(AUnit* Unit, EUnitLane Lane, int FormationIndex, const bool SendToLaneBegin)
{
	if (!Unit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Requested Lane Switch Without passing a unit or unit ptr invalid"));
		return false;
	}
	const int TeamIndex = Unit->GetTeamAffiliationIndex();
	auto* TeamLanes = FindAppropriateMapForTeamIndex(TeamIndex);
	if (!TeamLanes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unit's TeamIndex didn't return the team's lanes"));
		return false;
	}
	TObjectPtr<ALaneManager>* Selectedlane = TeamLanes->Find(Lane);
	if (!Selectedlane)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find selected lane"));
		return false;
	}
	if (Unit->GetLaneManager()->RemoveUnitFromLaneTracking(Unit))
	{
		Selectedlane->Get()->AddNewUnitToIndex(FormationIndex, Unit, SendToLaneBegin);
		return true;
	}
	return false;
}
