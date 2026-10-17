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
	
	HandleAttributeChange(FAttributeBase{.BaseValue = Damage}, UnitAttributeSet.GetHealth(),
		UnitAttributeSet.GetHealthAttribute());
	
	float FinalDamage = FMath::Abs(UnitAttributeSet.GetHealth().CurrentValue - HealthBeforeDamage);
	
	return FinalDamage;
}

void UAttributesComponent::InitializeAttributes()
{
	return;
}

void UAttributesComponent::ChangeAttributeCurrentValue(FName ChangedAttributeName, float NewValue)
{
	FProperty* Property = FAttributeSet::StaticStruct()->FindPropertyByName(ChangedAttributeName);
	if (!Property)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find the %s Attribute"), *ChangedAttributeName.ToString());
		return;
	}
	FStructProperty* StructProperty = CastField<FStructProperty>(Property);
	
	if (!StructProperty ||
		StructProperty->Struct != FAttributeBase::StaticStruct())
	{
		return;
	}
	
	FAttributeBase* Attribute =
		StructProperty->ContainerPtrToValuePtr<FAttributeBase>(&UnitAttributeSet);

	Attribute->CurrentValue = NewValue;
}

void UAttributesComponent::HandleAttributeChange(FAttributeBase NewValue, FAttributeBase Attribute, FAttributeBaseHandler AttributeHandler)
{
	/* This part is for a later extension of the attributes allowing different gameplay results 
	 * This part should be used as the main spot to handle attribute changes
	 */
	
	const float FinalHealth = 
		FMath::Clamp(Attribute.CurrentValue - NewValue.BaseValue, 0, Attribute.BaseValue);
	
	FAttributeBase UpdatedAttribute
	{.CurrentValue = FinalHealth, .BaseValue = Attribute.BaseValue};
	
	UnitAttributeSet.SetHealth(UpdatedAttribute, true);
	
	if (UpdatedAttribute.CurrentValue <= 0.0f)
	{
		HandleDeath();
	}
	OnAttributeChangedOnAttributeComp.Broadcast(UpdatedAttribute, AttributeHandler);
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

