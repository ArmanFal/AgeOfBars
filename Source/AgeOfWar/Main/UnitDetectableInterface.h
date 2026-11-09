#pragma once

#include "CoreMinimal.h"
#include "CustomComponents/Combat and Attributes/Combat.h"
#include "UnitDetectableInterface.generated.h"

UINTERFACE(BlueprintType)
class AGEOFWAR_API UUnitDetectableInterface : public UInterface
{
	GENERATED_BODY()
};

class AGEOFWAR_API IUnitDetectableInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EAffiliationToSelf GetAffiliationOfTarget(int TeamAffiliationIndex);
	
};
