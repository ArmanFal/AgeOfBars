// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "ProjectileBase.h"

#include "Main/Unit.h"
#include "Main/SubSystems/UnitSubsystem.h"
#include "Main/SubSystems/PoolSingleton/ProjectilePoolHandler.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
	ObjectPoolComp = CreateDefaultSubobject<UObjectPool>("ObjectPool");
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMinimalMovement>("ProjectileMovementComponent");
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	MainNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>("NiagaraSystem");
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	SetRootComponent(SphereComponent);
	
	MainNiagaraSystem->SetupAttachment(RootComponent);
	
	StaticMeshComponent->SetupAttachment(RootComponent);
	
	if (ensure(ObjectPoolComp))
	{
		ObjectPoolComp->OnOwnerPooled.AddDynamic(this, &AProjectileBase::HandlePoolStatus);
	}
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	/* Usually I would use another delegate but we want to expose some control here to blueprint side. 
	 * Temporarily this work for the main gameplay loop, consider lifetime management or some events might
	 * not work if owned by certain objects.
	 **/
	
	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AProjectileBase::HandleProjectileBeginOverLap);
	ProjectileMovementComponent->OnProjectileReachedTarget.AddUniqueDynamic(this, &AProjectileBase::InternalRequestDeinitialize);
	ProjectileMovementComponent->ProjectileStoppedMove.AddUniqueDynamic(this, &AProjectileBase::InternalRequestDeinitialize);
}

void AProjectileBase::InternalRequestDeinitialize()
{
	UProjectilePoolHandler* PoolHandlerSubsystem = GetWorld()->GetSubsystem<UProjectilePoolHandler>();
	if (PoolHandlerSubsystem)
	{
		IObjectPoolHandlerInterface::Execute_RequestReturnToPool(PoolHandlerSubsystem, GetProjectileOwnerAsUnit(), this);
	}
}

void AProjectileBase::HandleProjectileDeinitialization()
{
	UProjectilePoolHandler* PoolHandlerSubsystem = GetWorld()->GetSubsystem<UProjectilePoolHandler>();
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ObjectPoolComp->ReleaseIntoPool(PoolHandlerSubsystem);
	AssignProjectileOwner(nullptr);
	ProjectileMovementComponent->StopProjectileMove();
}

void AProjectileBase::HandleProjectileBeginOverLap(UPrimitiveComponent* OverlappedComponent,
                                                   AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == GetProjectileOwnerAsUnit() || OtherActor->IsA<AProjectileBase>())
	{
		return;
	}
	if (ProjectileResponseOnImpact)
	{
		ProjectileResponseOnImpact->GiveWorldContextToObject(GetWorld());
		ProjectileResponseOnImpact->HandleProjectileOverlapOnImpact(this, OtherActor);
		return;
	} 
	UE_LOG(LogTemp, Error, TEXT("The projectile's response on Impact was not assigned or called after deinitialization"));
	
	// switch (ProjectileResponseOnImpact)
	// {
	// 	
	// case EProjectileResponseOnImpact::NotAssigned :
	// 	
	// 	{
	// 		UE_LOG(LogTemp, Error, TEXT("The projectile's response on Impact was not assigned"));
	// 	}
	// 	
	// case EProjectileResponseOnImpact::SingleTarget:
	// 	
	// 	{
	// 	
	// 		AUnit* OtherUnit = Cast<AUnit>(OtherActor);
	//
	// 		if (OtherUnit && OtherUnit->GetTeamAffiliationIndex() != GetProjectileOwner()->GetTeamAffiliationIndex())
	// 		{
	// 			float FinalDamage = GetProjectileOwner()->GetUnitAttributes().GetDamage().CurrentValue;
	// 			IdamageableInterface::Execute_DealDamage(OtherUnit, FinalDamage);
	// 		}
	// 	}
	// 	
	// case EProjectileResponseOnImpact::AreaOfEffect:
	// 	
	// 	{
	// 		
	// 	}
	// }
}

void AProjectileBase::HandleProjectileInitialization(const UProjectileData* Data)
{
	if (!IsValid(Data))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot initialize %s: ProjectileData is invalid"), *GetName());
		return;
	}
	
	StaticMeshComponent->SetStaticMesh(Data->Mesh);
	MainNiagaraSystem->SetActive(false);
	SphereComponent->SetSphereRadius(Data->ProjectileCollisionRadius);
	SphereComponent->SetRelativeLocation(Data->ProjectileCollisionOffset);
	StaticMeshComponent->SetRelativeTransform(Data->MeshOffset);
	SetProjectileSpeed(Data->ProjectileSpeedMultiplier);
	ProjectileMovementComponent->SetProjectileArcParam(Data->ProjectileArc);
	if (Data->ProjectileResponsesOnImpact)
	{
		ProjectileResponseOnImpact = Data->ProjectileResponsesOnImpact;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Impact responses were not assigned"));
	}
	
	UProjectilePoolHandler* Subsystem = GetWorld()->GetSubsystem<UProjectilePoolHandler>();
	ObjectPoolComp->AcquireFromPool(Subsystem);
}

void AProjectileBase::ShootProjectile(FVector StartingLocation, FVector TargetLocation)
{
	SetActorLocation(StartingLocation);
	ProjectileMovementComponent->BeginProjectileMove(TargetLocation, ProjectileSpeed);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	OnProjectileShot.Broadcast();
}

AUnit* AProjectileBase::GetProjectileOwnerAsUnit() const
{
	AUnit* ReturnValue = Cast<AUnit>(ProjectileOwner);
	if (!ReturnValue)
		{
			return nullptr;
		} 
	return ReturnValue;
}

void AProjectileBase::AssignProjectileOwner(UObject* ProjOwner)
{
	ProjectileOwner = ProjOwner;
}

void AProjectileBase::HandlePoolStatus(bool IsInUse, UObject* PoolInstigator)
{
	if (!ensure(ProjectileMovementComponent))
	{
		return;
	}
	if (!IsInUse)
	{
		ProjectileMovementComponent->ElapsedProjectileLifeTime = 0.f;
	}
}


// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

