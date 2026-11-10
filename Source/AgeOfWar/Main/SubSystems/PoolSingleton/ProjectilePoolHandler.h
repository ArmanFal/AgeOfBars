// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Main/Managers/ObjectPoolHandlerInterface.h"
#include "Main/CustomComponents/Projectiles/ProjectileBase.h"
#include "Main/SubSystems/ProjectileSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "ProjectilePoolHandler.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedProjectilePoolInitialization);

UCLASS()
class AGEOFWAR_API UProjectilePoolHandler : public UWorldSubsystem, public IObjectPoolHandlerInterface
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FOnFinishedProjectilePoolInitialization OnFinishedProjectilePoolInitialization;
	
	UFUNCTION(BlueprintCallable)
	void BeginProjectilePoolInitialization(int PoolSize);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AProjectileBase*> GetAllSpawnedProjectiles() const {return SpawnedProjectiles;};
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AProjectileBase*> GetAllPooledProjectiles() const {return PooledProjectiles;};
	
	TArray<TObjectPtr<AProjectileBase>> PooledProjectiles;
	TArray<TObjectPtr<AProjectileBase>> SpawnedProjectiles;
	
#if WITH_EDITOR
	TArray<TObjectPtr<AProjectileBase>> AllDebugProjectiles;

	
	// Editor or Utility only 
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AProjectileBase*> GetAllProjectiles() const {return AllDebugProjectiles;};
	
#endif
protected:
	
	int ProjectilePoolSize{200};
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	static void HandleDeinitializationOfArray(TArray<TObjectPtr<AProjectileBase>> &Array);
	
	void InitializeProjectilePool();
	
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	
	virtual bool RequestReleaseFromPool_Implementation(FGameplayTag ReleaseObjectGameplayTag, UObject* RequestedBy, UObject*& ReleasedObject) override;;
	virtual bool RequestReturnToPool_Implementation(UObject* RequestingObject, UObject* ReturnedObject) override;
};
