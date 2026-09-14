// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPool.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOwnerPooled, bool, Status, UObject*, Instigator);

UCLASS()
class AGEOFWAR_API UObjectPool : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UObjectPool();
	
	bool AcquireFromPool(UObject* Instigator);
	bool ReleaseIntoPool(UObject* Instigator);
	
	UPROPERTY(BlueprintAssignable)
	FOwnerPooled OnOwnerPooled;
	
	UFUNCTION(BlueprintCallable)
	bool GetIsInUse() {return bIsInUse; };
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	bool bIsInUse{false};

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
