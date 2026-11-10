// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat.h"
#include "Main/Unit.h"
#include "Main/CustomComponents/Behaviours/Ability.h"

UCombat::UCombat()
{

}

bool UCombat::PerformAbility()
{
	if (!UnitAbility)
	{
		return false;
	}
	bool Activated = PerformUnitAction(UnitAbility);
	if (Activated)
	{
		bHasPerformedAbility = true;
		PerformingState = EUnitPerformingState::IsPerformingAbility;
	}
	return Activated;
}

bool UCombat::PerformAttack()
{
	bool Activated = PerformUnitAction(UnitAttackbehaviour);
	if (Activated)
	{
		PerformingState = EUnitPerformingState::IsPerformingAttack;
	}
	return Activated;
}

bool UCombat::PerformUnitAction(UAbility* AbilityReference) const
{
	if (AbilityReference)
	{
		AUnit* Owner = Cast<AUnit>(GetOwner());
		AbilityReference->TryActivateAbility(Owner, Owner->GetDetectedEnemyUnit());
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("Invalid Reference to Ability or Attack class"));
	return false;
}

bool UCombat::AssignUnitAbility(TSubclassOf<UAbility> Ability)
{
	bHasPerformedAbility = false;
	return AssignBehaviourToVarriable(Ability, UnitAbility);
}

bool UCombat::AssignAttackBehaviour(TSubclassOf<UAbility> Ability)
{
	return AssignBehaviourToVarriable(Ability, UnitAttackbehaviour);
}

bool UCombat::AssignBehaviourToVarriable(TSubclassOf<UAbility> AbilityClass, UAbility*& AssigningVarriable)
{
	if (AssigningVarriable)
	{
		// This should ensure there is no memory leaks
		AssigningVarriable->DeInitialize();
		AssigningVarriable = nullptr;
	}

	if (!IsValid(AbilityClass))
	{
		return false;
	}

	AssigningVarriable = NewObject<UAbility>(this, AbilityClass);
	bool IsAbilityValid = IsValid(AssigningVarriable);
	if (IsAbilityValid)
	{
		AssigningVarriable->Initialize();
	}
	return IsValid(AssigningVarriable);
}

// Called when the game starts
void UCombat::BeginPlay()
{
	Super::BeginPlay();
}

void UCombat::ChangeCombatState (EUnitDetectionState State)
{
	UnitState = State;
}


// Called every frame
void UCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

EAffiliationToSelf UCombat::GetDetectedUnitAffiliation(AUnit* CallerUnit, AUnit* OtherUnit)
{
	bool bIsAffiliated = CallerUnit->GetTeamAffiliationIndex() == OtherUnit->GetTeamAffiliationIndex();
	return bIsAffiliated ? EAffiliationToSelf::Friendly : EAffiliationToSelf::Enemy;
}

bool UCombat::PerformAbilityHasConditions() const
{
	return UnitAbility->PreConditionStatus == EAbilityPreConditions::HasConditions;
}





