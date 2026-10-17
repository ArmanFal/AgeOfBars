// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Main/Unit.h"
#include "Main/Managers/ObjectPoolHandlerInterface.h"
#include "Subsystems/WorldSubsystem.h"
#include "UnitPoolHandler.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedUnitPoolInitialization);

UCLASS()
class AGEOFWAR_API UUnitPoolHandler : public UWorldSubsystem, public IObjectPoolHandlerInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnFinishedUnitPoolInitialization OnFinishedUnitPoolInitialization;
	
	UFUNCTION(BlueprintCallable)
	void BeginUnitPoolInitialization(int PoolSize, TSubclassOf<AUnit> PoolClass);
	
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AUnit>> PooledUnits;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AUnit>> SpawnedUnits;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AUnit>> AllUnits;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AUnit> PooledUnitClass;
	
	int UnitsToPool{100};
	
private:
	
	void InitializeUnitPool();

	
	virtual bool RequestReleaseFromPool_Implementation(FGameplayTag ReleaseObjectGameplayTag, UObject* RequestedBy, UObject*& ReleasedObject) override;;
	virtual bool RequestReturnToPool_Implementation(UObject* RequestingObject, UObject* ReturnedObject) override;
	
};
