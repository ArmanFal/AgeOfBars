// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "LaneGridManager.h"


// Sets default values
ALaneGridManager::ALaneGridManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ALaneGridManager::GenerateCells()
{
	
}

// Called when the game starts or when spawned
void ALaneGridManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaneGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

