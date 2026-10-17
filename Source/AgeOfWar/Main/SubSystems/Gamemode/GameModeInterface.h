// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameModeInterface.generated.h"

/**
 * 
 */


class AUnit;
enum class EUnitLane : uint8;

UINTERFACE(BlueprintType)
class AGEOFWAR_API UGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

class AGEOFWAR_API IGameModeInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector RequestLocationOfLane(int TeamIndex, EUnitLane Lane);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool RequestSendUnitToNewLane(AUnit* Unit, EUnitLane Lane, int FormationIndex, const bool SendToLaneBegin);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool RequestSpawnUnit(int ForTeam, FGameplayTag UnitToSpawn, EUnitLane LaneToSpawn, FString &FailureLog);
};
