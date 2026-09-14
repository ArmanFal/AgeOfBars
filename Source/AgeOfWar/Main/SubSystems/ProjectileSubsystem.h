// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataManagingGameInstanceSubsystem.h"
#include "Main/Data/ProjectileData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ProjectileSubsystem.generated.h"

/**
 * 
 */

UCLASS()
class AGEOFWAR_API UProjectileSubsystem : public UDataManagingGameInstanceSubsystem
{
	GENERATED_BODY()
	
	int ProjectilePoolSize{300};
	
public:
	
	bool MapProjectileData();
	
	UFUNCTION(BlueprintCallable)
	const UProjectileData* GetProjectileData(const FGameplayTag& Tag) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UProjectileData*> GetAllProjectileData() const;

protected:
	
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UProjectileData>> AllProjectileData;
	
	static FGameplayTag GetProjectileTag(const UProjectileData* Data);
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

};
