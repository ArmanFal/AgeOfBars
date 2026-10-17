// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Conditions/AbilityConditions.h"
#include "UObject/Object.h"
#include "Ability.generated.h"

UENUM(BlueprintType)
enum class EAbilityPreConditions : uint8
{
	HasConditions,
	IsInstant,
	IsNotAssigned
};

UENUM(BlueprintType)
enum class EAbilityState : uint8
{
	Idle,
	Executing, 
	Pending
};

class AUnit;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class AGEOFWAR_API UAbility : public UObject
{
	GENERATED_BODY()
	
public:
	
	UAbility();
	
	UPROPERTY()
	AUnit* AbilityOwner;
	
	UPROPERTY(BlueprintReadWrite)
	EAbilityPreConditions PreConditionStatus{EAbilityPreConditions::IsNotAssigned};
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UAbilityConditions> ASYNC_Condition;
	
	void TryActivateAbility(AUnit* Caller, AUnit* PotentialTarget);
	
	void StopAbility();
	void Initialize();
	void DeInitialize();
	
	bool EvaluateConditions();
	
	// providing world context here to this particular UOBJECT
	virtual class UWorld* GetWorld() const override;
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void OnActivated();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	bool OnDeactivated();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void OnActivatedWithCondition(bool ConditionMet);
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void FinishAbility();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	AUnit* GetAbilityOwner() const { return AbilityOwner; };
	
	/*
	 * As per this version each ability can have only one async ability condition
	 */
	UPROPERTY()
	TObjectPtr<UAbilityConditions> ASYNC_AbilityCondition;

};
