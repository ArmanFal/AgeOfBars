// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Main/Data/AttributeSet.h"
#include "AttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedOnAttributeComponent, FAttributeBase, Attribute, FAttributeBaseHandler, AttributeHandler);
class UAttributeCalculator;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AGEOFWAR_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UAttributesComponent();
	
	UFUNCTION()
	void HandleAttributeChange(FAttributeBase NewValue, FAttributeBase Attribute, FAttributeBaseHandler AttributeHandler);
	
	UFUNCTION(BlueprintCallable)
	void InitializeAttributes();
	
	UFUNCTION(BlueprintCallable)
	void HandleDeath();
	
	UFUNCTION(BlueprintCallable)
	float ApplyDamage(float Damage);
	
	UFUNCTION(BlueprintCallable)
	void ChangeAttributeCurrentValue(FName ChangedAttributeName, float NewValue);
	
	const FAttributeSet* GetAttributeSet() const {return &UnitAttributeSet ; } ;
	// Blueprint getter only return a copy
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FAttributeSet GetAttributeSetCopy() {return UnitAttributeSet; }; 
	
	UFUNCTION(BlueprintCallable)
	void SetAttribute(const FAttributeSet Attribute);
	
	FOnDeath OnDeath;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedOnAttributeComponent OnAttributeChangedOnAttributeComp;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:

	FAttributeSet UnitAttributeSet;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
