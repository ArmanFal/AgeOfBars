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
UCLASS()
class AGEOFWAR_API UProjectileData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cosmetics")
	FGameplayTag ProjectileTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cosmetics")
	TObjectPtr<UStaticMesh> Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cosmetics")
	TObjectPtr<UNiagaraSystem> SpawnNiagaraEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cosmetics")
	TObjectPtr<UNiagaraSystem> ImpactNiagaraEffect;
};
