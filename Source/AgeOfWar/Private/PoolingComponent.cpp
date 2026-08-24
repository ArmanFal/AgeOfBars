// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingComponent.h"

// Sets default values for this component's properties
UPoolingComponent::UPoolingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool UPoolingComponent::ActivatePool(bool InUse)
{
	AActor* Poolable = GetOwner();

	if (Poolable == nullptr) return false; 

	Poolable->SetActorHiddenInGame(!InUse);
	Poolable->SetActorEnableCollision(InUse);
	Poolable->SetActorTickEnabled(InUse);
	
	return Poolable->IsActorTickEnabled() && !Poolable->IsHidden() && Poolable->GetRootComponent()->IsCollisionEnabled();
}
