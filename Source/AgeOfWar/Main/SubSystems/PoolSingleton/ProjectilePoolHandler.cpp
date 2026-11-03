// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "ProjectilePoolHandler.h"


void UProjectilePoolHandler::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UProjectilePoolHandler::BeginProjectilePoolInitialization(int PoolSize)
{
	ProjectilePoolSize = PoolSize;
	InitializeProjectilePool();
}

void UProjectilePoolHandler::InitializeProjectilePool()
{
	if (!PooledProjectiles.IsEmpty())
	{
		return;
	}
	for (int i = 0; i < ProjectilePoolSize; i++)
	{
		AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>();
		Projectile->HandlePoolStatus(false, this);
		PooledProjectiles.Add(Projectile);
		
#if WITH_EDITOR
		AllDebugProjectiles.Add(Projectile);
#endif
	}
	OnFinishedProjectilePoolInitialization.Broadcast();
}

void UProjectilePoolHandler::Deinitialize()
{
	Super::Deinitialize();
	HandleDeinitializationOfArray(PooledProjectiles);
	HandleDeinitializationOfArray(SpawnedProjectiles);
#if WITH_EDITOR
	HandleDeinitializationOfArray(AllDebugProjectiles);
#endif
}

void UProjectilePoolHandler::HandleDeinitializationOfArray(TArray<TObjectPtr<AProjectileBase>> &Array)
{
	for (auto Element : Array)
	{
		Element->Destroy();
	}
	Array.Reset();
}

bool UProjectilePoolHandler::RequestReleaseFromPool_Implementation(FGameplayTag ReleaseObjectGameplayTag, UObject* RequestedBy, UObject*& ReleasedObject)
{
	const UProjectileSubsystem* ProjectileDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UProjectileSubsystem>();
	if (!ProjectileDataSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile Data Subsystem Invalid At ProjectilePoolHandler"))
		return false;
	}
	const UProjectileData* ProjectileData = ProjectileDataSubsystem->GetProjectileData(ReleaseObjectGameplayTag);
	
	if (!ProjectileData)
	{
		UE_LOG(LogTemp, Error, TEXT("No projectile data found for tag: %s"), *ReleaseObjectGameplayTag.ToString());
		return false;
	}
	if (PooledProjectiles.IsEmpty())
	{	
		// Later handle spawning more if needed
		UE_LOG(LogTemp, Warning, TEXT("Projectile pool is empty. Requested tag: %s"), *ReleaseObjectGameplayTag.ToString());
		return false;
	}
	
	AProjectileBase* ChosenProjectile = PooledProjectiles.Pop(EAllowShrinking::No);
	
	SpawnedProjectiles.Add(ChosenProjectile);
	ReleasedObject = ChosenProjectile;
	
	ChosenProjectile->HandleProjectileInitialization(ProjectileData);
	ChosenProjectile->ProjectileTag = ProjectileData->ProjectileTag;
	if (RequestedBy)
	{
		ChosenProjectile->AssignProjectileOwner(RequestedBy);
	}
	
	return IsValid(ChosenProjectile) && IsValid(ReleasedObject);
}

bool UProjectilePoolHandler::RequestReturnToPool_Implementation(UObject* RequestingObject, UObject* ReturnedObject)
{
	if (!ReturnedObject)
	{
		UE_LOG(LogTemp, Error, TEXT("Returned Object Null At Projectile Pool Handling Implementation"));
		return false;
	}
	AProjectileBase* ReturnedProjectile = Cast<AProjectileBase>(ReturnedObject);
	if (!ReturnedProjectile)
	{
		UE_LOG(LogTemp, Error, TEXT("Attempted to return %s to projectile pool, but it is not an AProjectileBase"),
			*GetNameSafe(ReturnedObject));
		return false;
	}
	
	if (!SpawnedProjectiles.Contains(ReturnedProjectile))
	{
		UE_LOG(LogTemp, Error, TEXT("Attempted to return projectile %s, but it is not registered in SpawnedProjectiles, Executed by %s"),
			*GetNameSafe(ReturnedProjectile), *GetNameSafe(RequestingObject));
		return false;
	}
	
	UE_LOG(LogTemp, Warning,
	TEXT("RETURN ACCEPTED: %s | Requested by: %s"),
	*GetNameSafe(ReturnedProjectile),
	*GetNameSafe(RequestingObject));
	
	SpawnedProjectiles.Remove(ReturnedProjectile);
	PooledProjectiles.Add(ReturnedProjectile);
	
	ReturnedProjectile->HandleProjectileDeinitialization();
	ReturnedProjectile->AssignProjectileOwner(nullptr);
	ReturnedProjectile->HandlePoolStatus(true, this);
	ReturnedProjectile->ProjectileTag = FGameplayTag::EmptyTag;
	
	return true;
}

bool UProjectilePoolHandler::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}
