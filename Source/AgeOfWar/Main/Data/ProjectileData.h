// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "ProjectileData.generated.h"

/**
 * 
 */

class UProjectileResponseOnImpact;

UCLASS()
class AGEOFWAR_API UProjectileData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cosmetics")
	FGameplayTag ProjectileTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cosmetics")
	TObjectPtr<UStaticMesh> Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TMap<FGameplayTag, TObjectPtr<UNiagaraSystem>> CombatNiagaraSystems;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TMap<FGameplayTag, TObjectPtr<USoundBase>> Audios;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float ProjectileCollisionRadius;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float ProjectileSpeedMultiplier;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float ProjectileArc{0.5f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Cosmetics")
	TObjectPtr<UProjectileResponseOnImpact> ProjectileResponsesOnImpact{nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	FVector ProjectileCollisionOffset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cosmetics")
	bool ShowProjectileCollision{false};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cosmetics")
	FTransform MeshOffset;
};
