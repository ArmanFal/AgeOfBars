// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/TouchInterface.h"
#include "ObjectPoolHandlerInterface.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType)
class AGEOFWAR_API UObjectPoolHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

class AGEOFWAR_API IObjectPoolHandlerInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool RequestReturnToPool(UObject* RequestingObject);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool RequestReleaseFromPool(UObject* RequestingObject);
};
