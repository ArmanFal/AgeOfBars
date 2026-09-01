// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat.generated.h"

class UAttributesComponent;
class UAbility;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AGEOFWAR_API UCombat : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombat();

	UFUNCTION(BlueprintCallable, Category = "Components|Combat")
	bool PerformAbility();
	
	bool AssignAbility(TSubclassOf<UAbility> Ability);
	
	float CalcValuePerHitInstanceOfAbility(UAttributesComponent* Stats);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UAbility* UnitAbility = nullptr;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
