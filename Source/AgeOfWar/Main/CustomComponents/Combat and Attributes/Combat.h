// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat.generated.h"

struct FAttributeSet;

UENUM(BlueprintType)
enum class EUnitPerformingState : uint8
{
	IsPerformingAttack					UMETA(DisplayName = "IsPerformingAttack"),	
	IsPerformingAbility					UMETA(DisplayName = "IsPerformingAbility"),	
	NotPerforming					    UMETA(DisplayName = "NotPerforming")
};

UENUM(BlueprintType)
enum class EUnitDetectionState : uint8
{
	EnemiesOnlyDetected              UMETA(DisplayName = "Enemies Only Detected "),
	NoBlockingUnitDetected           UMETA(DisplayName = "No Blocking Unit Detected"),
	FriendlyOnlyDetected             UMETA(DisplayName = "Friendly Only Detected"),
	EnemyDetectedWhileBehindFriendly UMETA(DisplayName = "Enemy Detected While Behind Friendly"),
	UnknownState					 UMETA(DisplayName = "UnknownState")
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
	
	UFUNCTION(BlueprintCallable, Category = "Components|Combat")
	bool PerformAttack();
	
	bool AssignUnitAbility(TSubclassOf<UAbility> Ability);
	
	bool AssignAttackBehaviour(TSubclassOf<UAbility> Attackbehaviour);
	
	UFUNCTION(BlueprintCallable, Category = "Components|Combat")
	EUnitDetectionState GetCurrentState() {return UnitState; } ;
	
	UFUNCTION(BlueprintCallable, Category = "Components|Combat")
	EUnitPerformingState GetPerformingState() {return PerformingState; } ;
	
	UFUNCTION(BlueprintCallable, Category = "Components|Combat")
	void ChangeCombatState (EUnitDetectionState State);
	
	UFUNCTION(BlueprintCallable, Category = "Components|Combat")
	EUnitAffiliation GetDetectedUnitAffiliation(AUnit* CallerUnit, AUnit* OtherUnit);
	
	/*
	 * This condition could be cursor indicators to let user specify a location
	 * Before commiting the ability or any other conditions you may set.
	 */
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Components|Combat|Ability")
	bool PerformAbilityHasConditions() const;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components|Combat")
	bool bHasPerformedAbility = false;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UAbility* UnitAbility = nullptr;
	
	UPROPERTY()
	UAbility* UnitAttackbehaviour = nullptr;
	
	bool AssignBehaviourToVarriable(TSubclassOf<UAbility> AbilityClass, 
		UAbility*& AssigningVarriable);
	
	bool PerformUnitAction(UAbility* AbilityReference) const;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components|Combat")
	EUnitDetectionState UnitState{EUnitDetectionState::FriendlyOnlyDetected};

	UPROPERTY(BlueprintReadOnly, Category = "Components|Combat")
	EUnitPerformingState PerformingState;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
