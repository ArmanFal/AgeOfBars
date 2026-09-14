// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectPoolHandlerInterface.h"
#include "UObject/Object.h"
#include "Main/CustomComponents/Balancing/LanePowerCalculator.h"
#include "UnitLane.h"
#include "Main/Data/UUnitData.h"
#include "Components/BillboardComponent.h"
#include "LaneManager.generated.h"

class AUnit;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewUnitSpawned, AUnit*, NewUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewUnitDespawned, AUnit*, NewUnit);

UCLASS()
class AGEOFWAR_API ALaneManager : public AActor, public IObjectPoolHandlerInterface
{
	GENERATED_BODY()
	
	ALaneManager();
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnNewUnitSpawned OnNewUnitSpawned;
	
	UPROPERTY(BlueprintAssignable)
	FOnNewUnitDespawned OnNewUnitDespawned;
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnNewUnitSpawned(AUnit* NewUnit);
	
	UPROPERTY(BlueprintReadOnly)
	FTransform SpawnTransform{FTransform::Identity};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Billboards", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBillboardComponent> BillboardActorIndicator;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Billboards", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBillboardComponent> BillboardSpawnLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Balancing")
	TObjectPtr<ULanePowerCalculator> LanePowerCalculator;
	
	UPROPERTY(BlueprintReadWrite, Category = "Poolable Units")
	int UnitsToPool{100};
	
	UPROPERTY(BlueprintReadOnly, Category = "Poolable Units")
	TArray<TObjectPtr<AUnit>> PooledUnits;
	
	UPROPERTY(BlueprintReadOnly, Category = "Poolable Units")
	TArray<TObjectPtr<AUnit>> SpawnedUnits;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poolable Units")
	TSubclassOf<AUnit> PooledUnitClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poolable Units")
	EUnitLane UnitLane;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int TeamAffiliation{-1};
	
	UFUNCTION(BlueprintCallable, Category = "Team")
	int GetTeamAffiliation() const {return TeamAffiliation; } ;
	
	UFUNCTION(BlueprintCallable)
	AUnit* SpawnUnitOfTypeOnLane(UUnitData* UnitData);
	
	UFUNCTION(BlueprintCallable)
	void AddNewUnitToIndex(const int Index, AUnit* NewUnit);
	
	UFUNCTION(BlueprintCallable, Category = "Balancing")
	ULanePowerCalculator* GetLanePowerCalculator() { return LanePowerCalculator ? LanePowerCalculator : nullptr; } ;
	
	void BindDelegatesForLanePowerModification();
	
	UFUNCTION()
	void ModifyLanePower(AUnit* Source);
	
	virtual bool RequestReturnToPool_Implementation(UObject* RequestingObject) override;
	// virtual bool RequestReleaseFromPool_Implementation(AUnit* RequestingUnit) override;
	
	void HandleUnitPool(AUnit* PooledUnit);
	
	int GetUnitIndex(AUnit* Unit) const;
	
	virtual void BeginPlay() override;
};
