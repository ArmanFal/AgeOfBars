// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UCursorReceiveHandlerInterface.generated.h"

/**
 * 
 */

UINTERFACE(BlueprintType)
class AGEOFWAR_API UCursorReceiveHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

class AGEOFWAR_API ICursorReceiveHandlerInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NotifyCursorClicked(AActor* ClickedACtor, FVector ClickedWorldLocation);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NotifyCursorHover();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NotifyCursorLeaveHover();
};
