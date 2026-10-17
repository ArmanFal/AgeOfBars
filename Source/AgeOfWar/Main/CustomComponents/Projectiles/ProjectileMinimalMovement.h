// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "Components/ActorComponent.h"
#include "ProjectileMinimalMovement.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileStoppedMove);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileReachedTarget);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AGEOFWAR_API UProjectileMinimalMovement : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UProjectileMinimalMovement();
	
	void BeginProjectileMove(FVector TargetLocation, float ProjectileSpeed);
	void StopProjectileMove();
	
	UPROPERTY(BlueprintAssignable)
	FOnProjectileStoppedMove ProjectileStoppedMove;
	
	UPROPERTY(BlueprintAssignable)
	FOnProjectileReachedTarget OnProjectileReachedTarget;
	
	UPROPERTY(BlueprintReadOnly)
	float ElapsedProjectileLifeTime = 0.f;
	
	UPROPERTY(BlueprintReadOnly)
	float MaximumProjectileExistanceAllowed = 5.f;
	
	void SetProjectileArcParam(float ParamInput) {ArcParam = ParamInput;};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void StartProjectileTimer();
	float ProgressProjectileTimer(float deltaTime);
	
	float ProjectileSpeedMultiplier = 1.f;
	
	FVector ProjectileTargetLocation{FVector::ZeroVector};
	FVector Gravity{0.f,0.f,-980.f};
	
	float ArcParam{0.5f};
	FVector Velocity{FVector::ZeroVector};

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
