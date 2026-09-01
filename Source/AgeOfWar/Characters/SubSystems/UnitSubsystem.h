#pragma once

#include "Characters/Data/UUnitData.h"
#include "GameplayTagContainer.h"
#include "UnitSubsystem.generated.h"

UCLASS()
class AGEOFWAR_API UUnitSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	bool MapUnitData();
	
	UFUNCTION(BlueprintCallable)
	const UUnitData* GetUnitData(const FGameplayTag& Tag) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UUnitData*> GetAllUnitsData() const;

private:
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UUnitData>> UnitDataByTag;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
