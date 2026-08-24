// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Nexus.generated.h"

UCLASS()
class AGEOFWAR_API ANexus : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANexus();

	UPROPERTY(BlueprintReadWrite, Category = "Nexus")
	bool IsEnemy;

	UPROPERTY(EditAnywhere, Category = "Nexus")

	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Nexus")

	float CurrentHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
