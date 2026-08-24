// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "Ability.h"

UAbility::UAbility()
{
	
}

void UAbility::ActivateAbility() 
{
	OnActivated();
}

void UAbility::StopAbility()
{
	OnDeactivated();
}
