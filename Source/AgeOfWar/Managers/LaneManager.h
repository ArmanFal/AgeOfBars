// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LaneManager.generated.h"

class AUnit;

UCLASS()
class AGEOFWAR_API ULaneManager : public UObject
{
	GENERATED_BODY()
	
public:
	// UPROPERTY()
	// TArray<TUniquePtr<AUnit*>> PooledUnits;
	//
	// UPROPERTY()
	// TArray<TUniquePtr<AUnit*>> SpawnedUnits;
	
	// UFUNCTION(BlueprintCallable)
	// bool SpawnUnitOfType(AUnit* UnitToSpawn, int32 Lane);
};
