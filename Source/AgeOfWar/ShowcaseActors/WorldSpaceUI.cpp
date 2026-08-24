// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "WorldSpaceUI.h"

#include "Components/WidgetInteractionComponent.h"


// Sets default values
AWorldSpaceUI::AWorldSpaceUI()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComponent"));
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root")); 
	WidgetComponent->SetupAttachment(Root);
	WidgetInteraction->SetupAttachment(WidgetComponent);
}

void AWorldSpaceUI::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (WidgetComponent)
	{
		WidgetComponent->SetWidgetClass(WorldSpaceWidget);
	}
}

// Called when the game starts or when spawned
void AWorldSpaceUI::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWorldSpaceUI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

