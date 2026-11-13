// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomComponents/Enums/UnitLane.h"
#include "GameFramework/Actor.h"
#include "LaneGridManager.generated.h"

UCLASS()
class AGEOFWAR_API ALaneGridManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALaneGridManager();
	
	UPROPERTY(BlueprintReadWrite)
	EUnitLane BelongingLane;
	
	UPROPERTY(BlueprintReadWrite)
	int CellsPerLane = 50.f;

	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* GridMaterial;
	
	TArray<TWeakObjectPtr<AActor>> GridCell;
	
	UFUNCTION(BlueprintCallable, Category = "Overrides")
	void GenerateCells();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
