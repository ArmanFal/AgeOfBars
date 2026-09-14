// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Main/Managers//UnitLane.h"
#include "UObject/Object.h"
#include "LanePowerCalculator.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class AGEOFWAR_API ULanePowerCalculator : public UObject
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Balancing | Lanepower")
	float LanePower{0.f};
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Calculations")
	float ExecuteUnitWeightCalculation(AUnit* AffectingUnit, bool bIsFriendlyAffiliated);
	
	float CalculateFinalLaneUrgency(TArray<float>);
	
	UPROPERTY(BlueprintReadOnly, Category = "Balancing | Lanepower")
	TMap<EUnitLane, float> LaneUrgency;
	
private:
	
	float NexusDistance{0.f};
	
};
