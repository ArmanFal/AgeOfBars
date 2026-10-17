#pragma once


#include "CoreMinimal.h"
#include "UnitLane.generated.h"

UENUM(BlueprintType)
enum class EUnitLane : uint8
{
	Top,
	Middle,
	Bottom,
	UnAssigned
};
	