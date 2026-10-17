#include "UnitSubsystem.h"

#include "Engine/AssetManager.h"

void UUnitSubsystem::InitializeUnitData()
{
	if (!MapUnitData())
	{
		UE_LOG(LogTemp, Error, TEXT("Initialization of Unit Subsystem Failed"))
	}
}

const UUnitData* UUnitSubsystem::GetUnitData(const FGameplayTag& Tag) const
{
	return GetMappedData<UUnitData>(Tag, AllUnitData);
}

TArray<UUnitData*> UUnitSubsystem::GetAllUnitsData() const
{
	return GetAllDataOfType<UUnitData>(AllUnitData);
}

bool UUnitSubsystem::MapUnitData()
{
	return MapPrimaryAssets<UUnitData>(
		FPrimaryAssetType(TEXT("UnitData")),
		AllUnitData,
		UUnitSubsystem::GetUnitTag
	);
}

FGameplayTag UUnitSubsystem::GetUnitTag(const UUnitData* Data)
{
	return Data->UnitTag;
}

void UUnitSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (!MapUnitData())
	{
		UE_LOG(LogTemp, Error, TEXT("Initialization of Unit Subsystem Failed"))
	}
}
