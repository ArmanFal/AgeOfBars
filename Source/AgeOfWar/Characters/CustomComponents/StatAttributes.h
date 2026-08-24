// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/Attributes.h"
#include "StatAttributes.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AGEOFWAR_API UStatAttributes : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatAttributes();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attributes",
		meta = (ToolTip = "Set all the starting attributes of the character here"))
	FAttributes SpawningAttributes;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", 
		meta = (ToolTip = "Set all the starting attributes of the character here"))
	FAttributes UpdatedAttributes;
	
	UFUNCTION(BlueprintCallable)
	void InitializeAttributes();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
