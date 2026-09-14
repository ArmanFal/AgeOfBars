// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "ProjectileSubsystem.h"

void UProjectileSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

const UProjectileData* UProjectileSubsystem::GetProjectileData(const FGameplayTag& Tag) const
{
	return GetMappedData(Tag, AllProjectileData);
}

bool UProjectileSubsystem::MapProjectileData()
{
	return MapPrimaryAssets<UProjectileData>(
		FPrimaryAssetType(TEXT("Projectiles")),
		AllProjectileData,
		UProjectileSubsystem::GetProjectileTag
	);
}

FGameplayTag UProjectileSubsystem::GetProjectileTag(const UProjectileData* Data)
{
	return Data->ProjectileTag;
}


TArray<UProjectileData*> UProjectileSubsystem::GetAllProjectileData() const
{
	return GetAllDataOfType<UProjectileData>(AllProjectileData);
}


