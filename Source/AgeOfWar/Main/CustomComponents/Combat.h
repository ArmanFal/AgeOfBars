// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat.generated.h"

struct FAttributeSet;

UENUM(BlueprintType)
enum class EUnitDetectionState : uint8
{
	EnemiesOnlyDetected       UMETA(DisplayName = "Enemies Only Detected "),
	NoBlockingUnitDetected          UMETA(DisplayName = "No Blocking Unit Detected"),
	FriendlyOnlyDetected          UMETA(DisplayName = "Friendly Only Detected"),
	EnemyDetectedWhileBehindFriendly UMETA(DisplayName = "Enemy Detected While Behind Friendly"),
	UnknownState        UMETA(DisplayName = "UnknownState")
};

UENUM(BlueprintType)
enum class EUnitAffiliation : uint8
{
	Enemy UMETA(DisplayName = "Enemy"),
	Friendly UMETA(DisplayName = "Friendly"),
	Neutral UMETA(DisplayName = "Neutral"),
	UnknownAffiliation UMETA(DisplayName = "UnknownAffiliation")
};

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
	
	UFUNCTION(BlueprintCallable, Category = "Components|Combat")
	EUnitDetectionState GetCurrentState() {return UnitState; } ;
	
	UFUNCTION(BlueprintCallable, Category = "Components|Combat")
	void ChangeCombatState (EUnitDetectionState State);
	
	UFUNCTION(BlueprintCallable, Category = "Components|Combat")
	EUnitAffiliation GetDetectedUnitAffiliation(AUnit* CallerUnit, AUnit* OtherUnit);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UAbility* UnitAbility = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components|Combat")
	EUnitDetectionState UnitState{EUnitDetectionState::FriendlyOnlyDetected};

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
