#include "UnitSubsystem.h"

#include "Engine/AssetManager.h"

const UUnitData* UUnitSubsystem::GetUnitData(const FGameplayTag& Tag) const
{
	const TObjectPtr<UUnitData>* Found = UnitDataByTag.Find(Tag);
	
	return Found ? Found->Get() : nullptr;
}

TArray<UUnitData*> UUnitSubsystem::GetAllUnitsData() const
{
	TArray<UUnitData*> Results;
	
	for (const auto& M : UnitDataByTag)
	{
		Results.Add(M.Value.Get());
	}
	
	return Results;
}

bool UUnitSubsystem::MapUnitData()
{
	UnitDataByTag.Empty();
	
	UAssetManager& AssetManager = UAssetManager::Get();
	
	TArray<FPrimaryAssetId> AssetIds;
	
	AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType(TEXT("UnitData")), AssetIds);
	
	if (AssetIds.Num() == 0)
	{
		return false;
	}
	
	for (const FPrimaryAssetId& PrimaryAssetId : AssetIds)
	{
		const FSoftObjectPath Path = AssetManager.GetPrimaryAssetPath(PrimaryAssetId);
		
		UUnitData* UnitData = Cast<UUnitData>(Path.TryLoad());
		
		if (!UnitData)
		{
			continue;
		}
		if (!UnitData->UnitTag.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Unit %s Tag is invalid"), *UnitData->GetName());
			continue;
		}
		if (UnitDataByTag.Contains(UnitData->UnitTag))
		{
			UE_LOG(LogTemp, Error, TEXT("For %s Unit the tag is duplicated, Unit %s has the same tag"), *UnitData->GetName()
				, *UnitDataByTag.Find(UnitData->UnitTag)->GetName());
			continue;
		}
		UnitDataByTag.Add(UnitData->UnitTag, UnitData);
	}
	return UnitDataByTag.Num() > 0;
}

void UUnitSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (!MapUnitData())
	{
		UE_LOG(LogTemp, Error, TEXT("Initialization of Unit Subsystem Failed"))
	}
}
