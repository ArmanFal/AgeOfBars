// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MinimalMovement.generated.h"

DECLARE_DELEGATE(OnTickMovementDelegate)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AGEOFWAR_API UMinimalMovement : public UActorComponent
{
	
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMinimalMovement();
	
	OnTickMovementDelegate OnTickMovementDelegate;
	
	void MoveUnit(FVector Direction, float deltaTime);
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopMovement();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void BeginMovement();
	
	float MovementSpeed;
	FVector MovementDirection{0.f,0.f,0.f};
	
	UPROPERTY(transient)
	AActor* OwnerActor = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	bool bShouldBeginMovementOnBeginPlay{false};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	bool bShouldOverrideUnitDirection{false};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	FVector OverrideDirection{0.f, 0.f,0.f};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float Acceleration{0.f};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float MaxSpeed{500.f};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	FVector Velocity{0.f,0.f,0.f};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Collision Detection")
	float DelayBeforeEachDetectionSweep;
	
	void UpdateForwardDetection();
	
	bool EnsureOwnerExists() const;
	
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, 
		ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
