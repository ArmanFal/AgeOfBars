// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat.h"
#include "Main/Unit.h"
#include "Main/CustomComponents/Behaviours/Ability.h"

UCombat::UCombat()
{

}

bool UCombat::PerformAbility()
{
	if (UnitAbility)
	{
		UnitAbility->ActivateAbility(StaticCast<AUnit*>(GetOwner()));
		return true;
	}
	return false;
}

bool UCombat::AssignAbility(TSubclassOf<UAbility> Ability)
{
	if (UnitAbility)
	{
		UnitAbility->DeInitialize();
		UnitAbility = nullptr;
	}
	if (!IsValid(Ability))
	{
		return false;
	}
	UnitAbility = NewObject<UAbility>(this, Ability);
	return IsValid(UnitAbility);
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

EUnitAffiliation UCombat::GetDetectedUnitAffiliation(AUnit* CallerUnit, AUnit* OtherUnit)
{
	bool bIsAffiliated = CallerUnit->GetTeamAffiliationIndex() == OtherUnit->GetTeamAffiliationIndex();
	return bIsAffiliated ? EUnitAffiliation::Friendly : EUnitAffiliation::Enemy;
}





