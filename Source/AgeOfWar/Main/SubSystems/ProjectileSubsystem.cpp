// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "ProjectileSubsystem.h"

void UProjectileSubsystem::InitializeProjectileData()
{
	if (!MapProjectileData())
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't Find any Projectile Data"));
	}
}

const UProjectileData* UProjectileSubsystem::GetProjectileData(const FGameplayTag& Tag) const
{
	return GetMappedData(Tag, AllProjectileData);
}

bool UProjectileSubsystem::MapProjectileData()
{
	return MapPrimaryAssets<UProjectileData>(
		UProjectileData::StaticClass()->GetFName(),
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


