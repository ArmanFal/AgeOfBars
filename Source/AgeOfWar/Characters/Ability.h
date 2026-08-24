// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Unit.h"
#include "Ability.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class AGEOFWAR_API UAbility : public UObject
{
	GENERATED_BODY()
	
public:
	
	UAbility();
	
	UPROPERTY()
	TObjectPtr<AUnit> AbilityOwner;
	
	void ActivateAbility();
	void StopAbility();
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void OnActivated();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void OnDeactivated();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	AUnit* GetAbilityOwner() const {return AbilityOwner; };

};
