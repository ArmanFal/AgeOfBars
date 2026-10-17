// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Main/CustomComponents/Enums/UnitLane.h"
#include "Main/Managers/LaneManager.h"
#include "AgeOfBarsGameMode.generated.h"

/**
 * 
 */

UCLASS()
class AGEOFWAR_API AAgeOfBarsGameMode : public AGameModeBase, public IGameModeInterface
{
	GENERATED_BODY()
	
	TMap<int, TMap<EUnitLane, TObjectPtr<ALaneManager>>> LaneMap;
	
	void BeginPlay() override;
	void MapLanes();
	
	FVector GetLocationOfLane(int TeamIndex, EUnitLane Lane);
	
	//Helpers
	TMap<EUnitLane, TObjectPtr<ALaneManager>>* FindAppropriateMapForTeamIndex(int TeamIndex)
	{
		return LaneMap.Find(TeamIndex);
	};
	
	virtual FVector RequestLocationOfLane_Implementation(int TeamIndex, EUnitLane Lane) override;
	virtual bool RequestSpawnUnit_Implementation(int ForTeam, FGameplayTag UnitToSpawn, EUnitLane LaneToSpawn, FString &FailureLog) override;
	virtual bool RequestSendUnitToNewLane_Implementation(AUnit* Unit, EUnitLane Lane, int FormationIndex,  const bool SendToLaneBegin) override;
	
};
