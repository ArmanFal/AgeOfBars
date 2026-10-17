// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "CustomComponents/Parent/AttributeBase.h"
#include "CustomComponents/Projectiles/ProjectileBase.h"
#include "Data/ProjectileData.h"
#include "GameFramework/Actor.h"
#include "Managers/ObjectPoolHandlerInterface.h"
#include "Tavern.generated.h"

class UCombatLifeBar;

UCLASS()
class AGEOFWAR_API ATavern : public AActor
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
	
	TObjectPtr<UCombatLifeBar> CombatLifeBar;
	TObjectPtr<UWidgetComponent> WidgetComponent;
	TObjectPtr<UBoxComponent> CollisionBoxComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
