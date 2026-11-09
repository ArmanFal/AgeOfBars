// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "Tavern.h"


// Sets default values
ATavern::ATavern()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>("CollisionBoxComponent");
	AttributeComponent = CreateDefaultSubobject<UAttributesComponent>("AttributesComponent");
		
	SetRootComponent(CollisionBoxComponent);
	WidgetComponent->SetupAttachment(CollisionBoxComponent);
	
}

// Called when the game starts or when spawned
void ATavern::BeginPlay()
{
	Super::BeginPlay();
	CombatLifeBar = Cast<UCombatLifeBar>(WidgetComponent->GetUserWidgetObject());
	if (!AttributeComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Attribute Comp On Tavern"));
	}
	if (!CombatLifeBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat Life Bar On Tavern Invalid"));
	}
	else
	{
		if (AttributeComponent)
		{
			const float CHealth = AttributeComponent->GetAttributeSet()->GetHealth().CurrentValue;
			const float BHealth = AttributeComponent->GetAttributeSet()->GetHealth().BaseValue;
			CombatLifeBar->SetLifePercentage(CHealth / BHealth);
		}
	}
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

