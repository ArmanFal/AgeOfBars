// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UnitLane.h"
#include "Components/BillboardComponent.h"
#include "LaneManager.generated.h"

class AUnit;

UCLASS()
class AGEOFWAR_API ALaneManager : public AActor
{
	GENERATED_BODY()
	
	ALaneManager();
public:
	
	UPROPERTY(BlueprintReadOnly)
	FTransform SpawnTransform{FTransform::Identity};
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Billboards", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBillboardComponent> BillboardActorIndicator;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Billboards", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBillboardComponent> BillboardSpawnLocation;
	
	UPROPERTY(BlueprintReadWrite, Category = "Poolable Units")
	int UnitsToPool{100};
	
	UPROPERTY(BlueprintReadOnly, Category = "Poolable Units")
	TArray<TObjectPtr<AUnit>> PooledUnits;
	
	UPROPERTY(BlueprintReadOnly, Category = "Poolable Units")
	TArray<TObjectPtr<AUnit>> SpawnedUnits;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poolable Units")
	EUnitLane UnitLane;
	
	UFUNCTION(BlueprintCallable)
	bool SpawnUnitOfTypeOnLane();
	
	virtual void BeginPlay() override;
};
