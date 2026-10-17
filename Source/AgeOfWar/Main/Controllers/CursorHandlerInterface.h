// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CursorHandlerInterface.generated.h"

/**
 * 
 */

UINTERFACE(BlueprintType)
class AGEOFWAR_API UCursorHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

class AGEOFWAR_API ICursorHandlerInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RequestCursorDeselect();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RequestCursorIndicator(UObject* RequestBy, bool ShowCursor);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NotifyCursorClicked(FVector &ClickedWorldLocation);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetAllowLaneIndicatorDetectionByCursor(bool IsActive);
};
