// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "AbilityConditions.h"

void UAbilityConditions::MeetThisCondition(bool bConditionMet)
{
	OnConditionMet.ExecuteIfBound(bConditionMet);
}
