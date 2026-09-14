#pragma once

#include "DataManagingGameInstanceSubsystem.h"
#include "Main/Data/UUnitData.h"
#include "GameplayTagContainer.h"
#include "UnitSubsystem.generated.h"

UCLASS()
class AGEOFWAR_API UUnitSubsystem : public UDataManagingGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	bool MapUnitData();
	
	UFUNCTION(BlueprintCallable)
	const UUnitData* GetUnitData(const FGameplayTag& Tag) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UUnitData*> GetAllUnitsData() const;

private:
	
	static FGameplayTag GetUnitTag(const UUnitData* Data);
	
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UUnitData>> AllUnitData;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
