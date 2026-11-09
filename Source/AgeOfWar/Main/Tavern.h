// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatLifeBar.h"
#include "UnitDetectableInterface.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "CustomComponents/Combat and Attributes/AttributesComponent.h"
#include "CustomComponents/Parent/AttributeBase.h"
#include "CustomComponents/Projectiles/ProjectileBase.h"
#include "Data/ProjectileData.h"
#include "GameFramework/Actor.h"
#include "Managers/DamageableInterface.h"
#include "Managers/ObjectPoolHandlerInterface.h"
#include "Tavern.generated.h"

UCLASS()
class AGEOFWAR_API ATavern : public AActor,
	public IdamageableInterface,
	public IUnitDetectableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATavern();
	
	UPROPERTY(BlueprintReadWrite)
	int TeamAffiliationIndex{-1};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	FAttributeBase HealthAttribute;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCombatLifeBar> CombatLifeBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UWidgetComponent> WidgetComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> CollisionBoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAttributesComponent> AttributeComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
