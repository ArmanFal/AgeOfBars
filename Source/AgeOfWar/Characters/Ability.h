// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Unit.h"
#include "Ability.generated.h"

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
	
	void ActivateAbility(AUnit* Owner);
	void StopAbility();
	
	void DeInitialize();
	
	virtual class UWorld* GetWorld() const override;
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void OnActivated();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	bool OnDeactivated();
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void FinishAbility();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	AUnit* GetAbilityOwner() const {return AbilityOwner; };

};
