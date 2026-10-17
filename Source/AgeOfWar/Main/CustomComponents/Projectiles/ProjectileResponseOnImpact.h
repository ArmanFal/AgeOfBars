// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ProjectileResponseOnImpact.generated.h"

/**
 * 
 */

class AProjectileBase;

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class AGEOFWAR_API UProjectileResponseOnImpact : public UObject
{
	GENERATED_BODY()
	
public:
	
	void GiveWorldContextToObject(UWorld* WorldContext) { World = WorldContext;};
	
	UPROPERTY()
	TWeakObjectPtr<UWorld> World{nullptr};
	
	UFUNCTION(BlueprintImplementableEvent)
	void HandleProjectileOverlapOnImpact(AProjectileBase* Projectile, AActor* HitActor);
	
protected:
	virtual class UWorld* GetWorld() const override;
	
};
