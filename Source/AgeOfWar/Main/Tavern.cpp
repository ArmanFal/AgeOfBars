// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "Tavern.h"
#include "SubSystems/ProjectileSubsystem.h"


// Sets default values
ATavern::ATavern()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATavern::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATavern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// bool ATavern::RequestReleaseFromPool_Implementation(FGameplayTag ReleaseObjectGameplayTag, tsubUObject* RequestingObject, UObject*& ReleasedObject)
// {
// 	// 	if (!RequestingObject)
// 	// {
// 	// 	return false;
// 	// }
// 	//
// 	// AProjectileBase* Projectile = Cast<AProjectileBase>(RequestingObject);
// 	//
// 	// 	if (!Projectile)
// 	// {
// 	// 	UE_LOG(LogTemp, Warning, TEXT("Tavern Does Not Have Requested ObjectType."));
// 	// 	return false;
// 	// }
// 	//
// 	// const UProjectileSubsystem* ProjectileSubSystem = GetGameInstance()->GetSubsystem<UProjectileSubsystem>();
// 	//
// 	// 	if (!ProjectileSubSystem)
// 	// {
// 	// 	UE_LOG(LogTemp, Warning, TEXT("Projectile Subsystem is NULL."));
// 	// 	return false;
// 	// }
// 	//
// 	// const UProjectileData* Data = ProjectileSubSystem->GetProjectileData(ReleaseObjectGameplayTag);
// 	//
// 	// 	if (!Data)
// 	// {
// 	// 	UE_LOG(LogTemp, Warning, TEXT("Projectile Data is NULL."));
// 	// 	return false;
// 	// }
// 	//
// 	// Projectile->InitializeProjectile(Data); 
// 	// ReleasedObject = Projectile;
// 	// return true;
// }

