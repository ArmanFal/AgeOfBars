// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "Ability.h"

UAbility::UAbility()
{
	
}

void UAbility::DeInitialize()
{
	this->MarkAsGarbage();
}

void UAbility::ActivateAbility(AUnit* Owner) 
{
	AbilityOwner = Owner;
	OnActivated();
}

void UAbility::FinishAbility()
{
	// finish ability
}

void UAbility::StopAbility()
{
	OnDeactivated();
}

UWorld* UAbility::GetWorld() const
{
	if (AActor* Owner = GetTypedOuter<AActor>())
	{
		return Owner->GetWorld();
	}
	return nullptr;
}