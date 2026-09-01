// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat.h"
#include "Characters/Ability.h"


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

// Sets default values for this component's properties
UCombat::UCombat()
{

}


// Called when the game starts
void UCombat::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

