// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileMinimalMovement.h"
#include "GameFramework/Actor.h"
#include "Main/CustomComponents/ObjectPool.h"
#include "Main/Data/ProjectileData.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "ProjectileResponseOnImpact.h"
#include "ProjectileBase.generated.h"

class AUnit;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileBeginOverLap, AActor*, Target, AProjectileBase*, Projectile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileShot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileDeinitialized);

UCLASS()
class AGEOFWAR_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AProjectileBase();
	
	UFUNCTION(BlueprintCallable)
	void ShootProjectile(FVector StartingLocation, FVector TargetLocation);

	void HandleProjectileInitialization(const UProjectileData* Data);
	
	UFUNCTION()
	void HandlePoolStatus(bool Status, UObject* PoolInstigator);
	
	UPROPERTY(BlueprintAssignable)
	FOnProjectileBeginOverLap OnProjectileBeginOverLap;
	
	UPROPERTY(BlueprintAssignable)
	FOnProjectileShot OnProjectileShot;

	UPROPERTY(BlueprintAssignable)
	FOnProjectileDeinitialized OnProjectileDeinitialized;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ProjectileTag;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AUnit* GetProjectileOwnerAsUnit() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UObject* GetProjectileOwnerAsObject() const {return ProjectileOwner;};
	
	void AssignProjectileOwner(UObject* ProjOwner);
	
	UFUNCTION(BlueprintCallable)
	void InternalRequestDeinitialize();
	
	UFUNCTION()
	void HandleProjectileDeinitialization();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void SetProjectileSpeed(float speed) {ProjectileSpeed = speed; };
	
	UPROPERTY()
	UObject* ProjectileOwner;
	
	FVector ProjectileTarget = FVector{0.f,0.f,0.f};
	
	float ProjectileSpeed = 50.f;
	
	UPROPERTY()
	UProjectileResponseOnImpact* ProjectileResponseOnImpact;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UObjectPool> ObjectPoolComp;
	
	UPROPERTY(BlueprintReadOnly)
	UProjectileData* ProjectileData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UProjectileMinimalMovement> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UNiagaraComponent> MainNiagaraSystem;
	
	UFUNCTION()
	void HandleProjectileBeginOverLap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
