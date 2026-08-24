// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorldSpaceWidgets.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "WorldSpaceUI.generated.h"

UCLASS()
class AGEOFWAR_API AWorldSpaceUI : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldSpaceUI();
	
	UPROPERTY()
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetInteractionComponent> WidgetInteraction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UWorldSpaceWidgets> WorldSpaceWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
