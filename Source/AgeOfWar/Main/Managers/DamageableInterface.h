// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType, Blueprintable)
class AGEOFWAR_API UdamageableInterface : public UInterface
{
	GENERATED_BODY()
};

class AGEOFWAR_API IdamageableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	bool DealDamage(float Damage);
};
