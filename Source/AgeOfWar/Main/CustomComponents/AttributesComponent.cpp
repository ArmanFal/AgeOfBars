// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesComponent.h"


// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UAttributesComponent::ApplyDamage(float Damage)
{
	float HealthBeforeDamage = UnitAttributeSet.GetHealth().CurrentValue;
	
	HandleAttributeChange(FAttributeBase{.BaseValue = Damage}, UnitAttributeSet.GetHealth());
	
	float FinalDamage = FMath::Abs(UnitAttributeSet.GetHealth().CurrentValue - HealthBeforeDamage);
	
	return FinalDamage;
}

void UAttributesComponent::InitializeAttributes()
{
	return;
}

void UAttributesComponent::HandleAttributeChange(FAttributeBase NewValue, FAttributeBase Attribute)
{
	/* This part is for a later extension of the attributes allowing different gameplay results */
	
	const float FinalHealth = 
		FMath::Clamp(Attribute.CurrentValue- NewValue.BaseValue, 0, Attribute.BaseValue);
	UnitAttributeSet.SetHealth(FAttributeBase{.CurrentValue = FinalHealth}, true);
	
	OnAnyAttributeChanged.Broadcast();	
}

void UAttributesComponent::SetAttribute(const FAttributeSet Attribute)
{
	UnitAttributeSet = Attribute;
	UnitAttributeSet.SetAllCurrentValuesToBaseValue();
}

// Called when the game starts
void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributesComponent::HandleDeath()
{
	OnDeath.Broadcast();
}

