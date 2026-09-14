// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "ProjectileBase.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
	ObjectPool = CreateDefaultSubobject<UObjectPool>("ObjectPool");
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

