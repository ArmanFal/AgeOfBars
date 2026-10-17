// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectPoolHandlerInterface.h"
#include "UObject/Object.h"
#include "Main/CustomComponents/Balancing/LanePowerCalculator.h"
#include "Main/CustomComponents/Enums/UnitLane.h"
#include "Main/Data/UUnitData.h"
#include "Components/BillboardComponent.h"
#include "LaneManager.generated.h"

class AUnit;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewUnitSpawned, AUnit*, NewUnit, bool, SpawnFromLaneBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewUnitDespawned, AUnit*, NewUnit);

UCLASS()
class AGEOFWAR_API ALaneManager : public AActor
{
	GENERATED_BODY()
	
	ALaneManager();
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnNewUnitSpawned OnNewUnitSpawned;
	
	UPROPERTY(BlueprintAssignable)
	FOnNewUnitDespawned OnNewUnitDespawned;
	
	UPROPERTY(BlueprintReadOnly)
	FTransform SpawnTransform{FTransform::Identity};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Billboards", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBillboardComponent> BillboardActorIndicator;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Billboards", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBillboardComponent> BillboardSpawnLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Balancing")
	TObjectPtr<ULanePowerCalculator> LanePowerCalculator;
	
	UPROPERTY(BlueprintReadOnly, Category = "Poolable Units")
	TArray<AUnit*> SpawnedUnits;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poolable Units")
	TSubclassOf<AUnit> PooledUnitClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poolable Units")
	EUnitLane UnitLane;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int TeamAffiliation{-1};
	
	UFUNCTION(BlueprintCallable, Category = "Team")
	int GetTeamAffiliation() const {return TeamAffiliation; } ;
	
	UFUNCTION(BlueprintCallable, Category = "Team")
	bool RemoveUnitFromLaneTracking(AUnit* Unit);
	
	UFUNCTION(BlueprintCallable)
	bool AddNewUnitToIndex(const int Index, AUnit* NewUnit, const bool SpawnFromLaneBegin);
	
	UFUNCTION()
	void ModifyLanePowerNewUnitSpawnedWrapper(AUnit* NewUnit, const bool SpawnFromLaneBegin);
	
	UFUNCTION(BlueprintCallable, Category = "Balancing")
	ULanePowerCalculator* GetLanePowerCalculator() { return LanePowerCalculator ? LanePowerCalculator : nullptr; } ;
	
	void BindDelegatesForLanePowerModification();
	
	UPROPERTY(BlueprintReadOnly)
	AActor* OwningTavern;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetAffiliatedTavern() {return OwningTavern;};
	
	UFUNCTION()
	void ModifyLanePower(AUnit* Source);
	
	void HandleUnitPool(AUnit* PooledUnit);
	
	UFUNCTION(BlueprintCallable, BLueprintPure)
	int GetUnitIndex(AUnit* Unit) const;
	
	virtual void BeginPlay() override;
};
