// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "ProjectileMinimalMovement.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UProjectileMinimalMovement::UProjectileMinimalMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UProjectileMinimalMovement::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UProjectileMinimalMovement::BeginProjectileMove(FVector TargetLocation, float ProjectileSpeed)
{
	ProjectileTargetLocation = TargetLocation;
	if (!UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetOwner()->GetWorld(), Velocity,
		GetOwner()->GetActorLocation(), ProjectileTargetLocation,  Gravity.Z, ArcParam))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not Suggest a Projectile Velocity"));
		StopProjectileMove();
		OnProjectileReachedTarget.Broadcast();
		return;
	}
	
	Velocity *= ProjectileSpeed;
	ProjectileSpeedMultiplier = ProjectileSpeed;
	
	SetComponentTickEnabled(true);
	StartProjectileTimer();
}

void UProjectileMinimalMovement::StopProjectileMove()
{
	ProjectileStoppedMove.Broadcast();
	ProjectileTargetLocation = FVector::ZeroVector;
	Velocity = FVector::ZeroVector;
	SetComponentTickEnabled(false);
}

// Called every frame
void UProjectileMinimalMovement::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ProjectileTargetLocation.Equals(GetOwner()->GetActorLocation(), 5.f) || 
		ProgressProjectileTimer(DeltaTime) > MaximumProjectileExistanceAllowed)
	{
		ElapsedProjectileLifeTime = 0.f;
		OnProjectileReachedTarget.Broadcast();
		StopProjectileMove();
		return;
	}
	FRotator UpdatedRotation = FRotator::ZeroRotator;
	
	Velocity +=  Gravity * FMath::Square(ProjectileSpeedMultiplier) * DeltaTime;
	
	UpdatedRotation = Velocity.Rotation();
	GetOwner()->AddActorWorldOffset(Velocity * DeltaTime);
	GetOwner()->SetActorRotation(UpdatedRotation);
}
float UProjectileMinimalMovement::ProgressProjectileTimer(float DeltaTime)
{
	return ElapsedProjectileLifeTime += DeltaTime;
}
void UProjectileMinimalMovement::StartProjectileTimer()
{
	ElapsedProjectileLifeTime = 0.0f;
}


