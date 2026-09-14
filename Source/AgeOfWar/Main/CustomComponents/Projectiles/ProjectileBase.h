// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Main/CustomComponents/ObjectPool.h"
#include "ProjectileBase.generated.h"

UCLASS()
class AGEOFWAR_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<UObjectPool> ObjectPool;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
