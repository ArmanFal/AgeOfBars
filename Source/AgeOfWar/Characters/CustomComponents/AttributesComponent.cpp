// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesComponent.h"

#include "Characters/SubSystems/AttributeCalculator.h"


// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UAttributesComponent::ApplyDamage(float Damage)
{
	const float FinalHealth = 
		FMath::Clamp(RealTimeAttributes.GetHealth().Value- Damage, 0, BaseAttributes.GetHealth().Value);
	
	RealTimeAttributes.SetHealth(FAttributeBase{FinalHealth}, true);
	return Damage;
}

void UAttributesComponent::InitializeAttributes()
{
	RealTimeAttributes = BaseAttributes;
	RealTimeAttributes.OnAttributeChanged.AddDynamic(this, &UAttributesComponent::HandleAttributeChange);
}

void UAttributesComponent::HandleAttributeChange(FAttributeBase NewValue, FAttributeBaseHandler Attribute)
{
	// This part is for a later extension of the attributes allowing different gameplay results //
	//UAttributeCalculator* Calculator = GetWorld()->GetGameInstance()->GetSubsystem<UAttributeCalculator>();
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

