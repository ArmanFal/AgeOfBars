// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataManagingGameInstanceSubsystem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDataLoaded);

UCLASS()
class AGEOFWAR_API UDataManagingGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	
	FOnDataLoaded OnDataLoaded;
	
	template<typename T>
	bool MapPrimaryAssets(const FPrimaryAssetType& AssetType, TMap<FGameplayTag, TObjectPtr<T>>& OutMap,
		TFunctionRef<FGameplayTag(const T*)> GetTag) const
	{
		// making sure we don't map assets repeatedly
		OutMap.Empty();
		
		UAssetManager& AssetManager = UAssetManager::Get();
		
		TArray<FPrimaryAssetId> AssetIDs;
		AssetManager.GetPrimaryAssetIdList(AssetType, AssetIDs);
		
		if (AssetIDs.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("No Primary Assets Found For %s"), *AssetType.ToString());
			return false;
		}
		
		// For each asset we find map them into their appropriate tags this is usually set by the designer in 
		// Data assets if a data tag is duplicated or not assigned you may receive a console error.
		for (const FPrimaryAssetId& PrimaryAssetId : AssetIDs)
		{
			const FSoftObjectPath Path = AssetManager.GetPrimaryAssetPath(PrimaryAssetId);
			
			T* Data = Cast<T>(Path.TryLoad());
			
			if (!Data)
			{
				continue;
			}
			
			FGameplayTag DataTag = GetTag(Data);
			
			if (!DataTag.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Data Tag Not Found For %s"), *Data->GetName());
				continue;
			}
			
			if (const TObjectPtr<T>* ExistingAsset = OutMap.Find(DataTag))
			{
				UE_LOG(LogTemp, Warning, TEXT("Duplicate Tag Found On %s, %s Already has The %s Tag,"
								  "Ensure each Tag is Unique To Each Data Asset"), 
					*Data->GetName(), *ExistingAsset->Get()->GetName(), *DataTag.ToString());
				continue;
			}
			
			OutMap.Add(DataTag, Data);
		}
		
		return !OutMap.IsEmpty();
	}
	
	// Helper templates for each data getters
	
	template<typename T>
	const T* GetMappedData(const FGameplayTag& Tag, const TMap<FGameplayTag, TObjectPtr<T>>& DataMap) const
	{
		if (const TObjectPtr<T>* Found = DataMap.Find(Tag))
		{
			return (*Found);
		}
		return nullptr;
	}
	
	template<typename T>
	TArray<TObjectPtr<T>> GetAllDataOfType( const TMap<FGameplayTag, TObjectPtr<T>>& DataMap) const
	{
		TArray<TObjectPtr<T>> Result;
		Result.Reserve(DataMap.Num());
		
		for (const auto& M : DataMap)
		{
			Result.Add(M.Value.Get());
		}
		
		return Result;
	}
	
};
