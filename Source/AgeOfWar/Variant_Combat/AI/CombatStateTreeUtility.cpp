// Copyright Epic Games, Inc. All Rights Reserved.


#include "CombatStateTreeUtility.h"
#include "Kismet/GameplayStatics.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeExecutionTypes.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Nexus.h"
#include "CombatEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "StateTreeAsyncExecutionContext.h"

bool FStateTreeCharacterGroundedCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	// is the character currently grounded?
	bool bCondition = InstanceData.Character->GetMovementComponent()->IsMovingOnGround();

	return InstanceData.bMustBeOnAir ? !bCondition : bCondition;
}

#if WITH_EDITOR
FText FStateTreeCharacterGroundedCondition::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting /*= EStateTreeNodeFormatting::Text*/) const
{
	return FText::FromString("<b>Is Character Grounded</b>");
}
#endif // WITH_EDITOR

EStateTreeRunStatus FStateTreeFaceActorTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	// have we transitioned from another state?
	if (Transition.ChangeType == EStateTreeStateChangeType::Changed)
	{
		// get the instance data
		FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

		// set the AI Controller's focus
		InstanceData.Controller->SetFocus(InstanceData.ActorToFaceTowards);
	}

	return EStateTreeRunStatus::Running;
}

void FStateTreeFaceActorTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	// have we transitioned to another state?
	if (Transition.ChangeType == EStateTreeStateChangeType::Changed)
	{
		// get the instance data
		FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

		// clear the AI Controller's focus
		InstanceData.Controller->ClearFocus(EAIFocusPriority::Gameplay);
	}
}

#if WITH_EDITOR
FText FStateTreeFaceActorTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting /*= EStateTreeNodeFormatting::Text*/) const
{
	return FText::FromString("<b>Face Towards Actor</b>");
}
#endif // WITH_EDITOR


////////////////////////////////////////////////////////////////////

EStateTreeRunStatus FStateTreeSetCharacterSpeedTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	// have we transitioned from another state?
	if (Transition.ChangeType == EStateTreeStateChangeType::Changed)
	{
		// get the instance data
		FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

		// set the character's max ground speed
		InstanceData.Character->GetCharacterMovement()->MaxWalkSpeed = InstanceData.Speed;
	}

	return EStateTreeRunStatus::Running;
}

#if WITH_EDITOR
FText FStateTreeSetCharacterSpeedTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting /*= EStateTreeNodeFormatting::Text*/) const
{
	return FText::FromString("<b>Set Character Speed</b>");
}
#endif // WITH_EDITOR

////////////////////////////////////////////////////////////////////

EStateTreeRunStatus FStateTreeGetPlayerInfoTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	// get the instance data
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	// get the character possessed by the first local player
	InstanceData.TargetPlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(InstanceData.Character, 0));

	// do we have a valid target?
	if (InstanceData.TargetPlayerCharacter)
	{
		// update the last known location
		InstanceData.TargetPlayerLocation = InstanceData.TargetPlayerCharacter->GetActorLocation();
	}

	// update the distance
	InstanceData.DistanceToTarget = FVector::Distance(InstanceData.TargetPlayerLocation, InstanceData.Character->GetActorLocation());

	return EStateTreeRunStatus::Running;
}

#if WITH_EDITOR
FText FStateTreeGetPlayerInfoTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting /*= EStateTreeNodeFormatting::Text*/) const
{
	return FText::FromString("<b>Get Player Info</b>");
}
#endif // WITH_EDITOR

EStateTreeRunStatus FStateFreeFindOpposingNexus::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	// making sure we use the proper Data Type
	FInstanceDataType& Instance = Context.GetInstanceData(*this);

	UWorld* World = Context.GetWorld();

	if (!World)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(World, ANexus::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		for (AActor* Actor : FoundActors)
		{
			ANexus* Nexus = Cast<ANexus>(Actor); // Nexus is very close to a typical actor no memory issues

			if (Nexus)
			{
				if (Nexus->IsEnemy != Instance.IsEnemy)
				{
					Instance.FoundNexus = Nexus;
					return EStateTreeRunStatus::Succeeded;
				}
			}
		}
	}

	Instance.FoundNexus = nullptr;
	return EStateTreeRunStatus::Failed;
}

#if WITH_EDITOR
FText FStateFreeFindOpposingNexus::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup,
	EStateTreeNodeFormatting Formatting) const
{
	if (const FInstanceDataType* Data = InstanceDataView.GetPtr<FInstanceDataType>()) 

	{
		return FText::FromString(Data->IsEnemy ? TEXT("Find Enemy Nexus") : TEXT("Find Ally Nexus"));
	}

	return FText::FromString(TEXT("Find Opposing Nexus"));

}
#endif

EStateTreeRunStatus FStateTreeCharacterWait::EnterState(FStateTreeExecutionContext& Context, 
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& Instance = Context.GetInstanceData(* this);

	if (!IsValid(Instance.Character)) return EStateTreeRunStatus::Failed;

	return EStateTreeRunStatus::Succeeded;

}