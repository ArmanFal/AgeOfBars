// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbilityConditions.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(OnConditionMet, bool);

UCLASS(Abstract, BlueprintType, Blueprintable)
class AGEOFWAR_API UAbilityConditions : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool ConditionMet{false};
	
public:
	
	OnConditionMet OnConditionMet;
	
	UFUNCTION(BlueprintCallable)
	void MeetThisCondition(bool bConditionMet);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ExecuteCondition();
};
