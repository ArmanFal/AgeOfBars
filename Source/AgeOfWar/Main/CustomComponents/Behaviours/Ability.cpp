// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "Ability.h"

UAbility::UAbility()
{
	
}

void UAbility::Initialize()
{
	if (PreConditionStatus == EAbilityPreConditions::IsNotAssigned)
	{
		UE_LOG(LogTemp, Error, 
			TEXT("%s Ability has no PreCondition Assigned ensure EAbilityPreconditions Enum variable is assigned"), *GetName());
		return;
	}
	if (PreConditionStatus != EAbilityPreConditions::HasConditions)
	{
		return;
	}
	if (ASYNC_Condition)
	{
		ASYNC_AbilityCondition = NewObject<UAbilityConditions>(this, ASYNC_Condition);
	}
}

void UAbility::DeInitialize()
{
	this->MarkAsGarbage();
}

bool UAbility::EvaluateConditions()
{
	if (PreConditionStatus != EAbilityPreConditions::HasConditions
		|| !ASYNC_AbilityCondition)
	{
		return true;
	}
	
	ASYNC_AbilityCondition->OnConditionMet.BindUObject(this, &UAbility::OnActivatedWithCondition);
	ASYNC_AbilityCondition->ExecuteCondition();
	
	if (ASYNC_AbilityCondition->OnConditionMet.IsBoundToObject(this))
	{
		return true;
	}
	return false;
}

void UAbility::TryActivateAbility(AUnit* Caller, AUnit* PotentialTarget) 
{
	AbilityOwner = Caller;
	if (PreConditionStatus == EAbilityPreConditions::IsNotAssigned)
	{
		UE_LOG(LogTemp, Error, 
			TEXT("%s Ability has no PreCondition Assigned ensure EAbilityPreconditions Enum variable is assigned"), *GetName());
		return;
	}
	OnActivated();
	// EvaluateConditions();
}

void UAbility::FinishAbility()
{
	if (ASYNC_AbilityCondition)
	{
		ASYNC_AbilityCondition->OnConditionMet.Unbind();
	}
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