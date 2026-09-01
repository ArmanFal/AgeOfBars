// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/Data/AttributeSet.h"
#include "AttributesComponent.generated.h"

#define ATTRIBUTE_ACCESSORS(Type, Name)\
UFUNCTION(BlueprintReadOnly)		\
Type Get##Name() const			\
{										\
return Name;							\
}										\
UFUNCTION(BlueprintCallable)			\
void Set##Name(Type InValue)			\
{										\
Name = InValue;							\
}	

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

class UAttributeCalculator;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AGEOFWAR_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttributesComponent();
	
	UFUNCTION()
	void HandleAttributeChange(FAttributeBase NewValue, FAttributeBaseHandler Attribute);
	
	UFUNCTION(BlueprintCallable)
	void InitializeAttributes();
	
	UFUNCTION(BlueprintCallable)
	void HandleDeath();
	
	UFUNCTION(BlueprintCallable)
	float ApplyDamage(float Damage);
	
	FOnDeath OnDeath;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:

	FAttributeSet BaseAttributes;
	FAttributeSet RealTimeAttributes;
	
public:
	
	ATTRIBUTE_ACCESSORS(FAttributeSet, RealTimeAttributes)
	ATTRIBUTE_ACCESSORS(FAttributeSet, BaseAttributes)
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
#undef ATTRIBUTE_ACCESSORS
