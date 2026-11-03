// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "MainGameplayController.h"

#include "ToolContextInterfaces.h"
#include "UCursorReceiveHandlerInterface.h"

void AMainGameplayController::BeginPlay()
{
	Super::BeginPlay();
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		if (!DefaultMappingContext)
		{
			UE_LOG(LogTemp, Error, TEXT("No Input Mapping was Assigned to %s"), *GetName())
		}
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
	if (SelectCursorActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride =
	   ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		SelectCursorActorObject = GetWorld()->SpawnActor<AActor>(SelectCursorActorClass, SpawnParams);
		SelectCursorActorObject->SetHidden(true);
	}
}

void AMainGameplayController::DetectUnderCursor(float DeltaTime)
{
	if (DeltaTime < KINDA_SMALL_NUMBER)
	{
		return;
	}
	AUnit* NewUnit = nullptr;
	
	// Unit Detection Trace
	if (GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
		false,
		CursorHitResult))
	{
		NewUnit = Cast<AUnit>(CursorHitResult.GetActor());
		if (!NewUnit && bShouldSelectLaneIndicator)
		{
			AActor* HoveredActorCandidate = nullptr;
			HoveredActorCandidate = CursorHitResult.GetActor();
			if (HoveredActorCandidate)
			{
				if (HoveredActor != HoveredActorCandidate)
				{
					if (HoveredActor)
					{
						ICursorReceiveHandlerInterface::Execute_NotifyCursorLeaveHover(HoveredActor);
					}
					if (HoveredActorCandidate->GetClass()->ImplementsInterface(UCursorReceiveHandlerInterface::StaticClass()))
					{
						HoveredActor = HoveredActorCandidate;
						ICursorReceiveHandlerInterface::Execute_NotifyCursorHover(HoveredActor);
					}
				}
			}
		} 
	}	
	else if (HoveredActor)
	{
		ICursorReceiveHandlerInterface::Execute_NotifyCursorLeaveHover(HoveredActor);
	}
	
	FHitResult Result;
	// Anything else visible Trace
	if (GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		Result))
	{
		
		LocationUnderCursor = Result.Location;
	}
	
	
	// Nothing changed
	if (NewUnit == CursorUnit)
	{
		return;
	}

	// Disable previous hovered unit
	if (IsValid(CursorUnit))
	{
		CursorUnit->DisableOverlayMaterial(HoverOverlayMaterial);
	}

	// Assign new hovered unit
	CursorUnit = NewUnit;

	// Enable new hovered unit
	if (IsValid(CursorUnit))
	{
		if (!bHasSelectedAUnit)
		{
			CursorUnit->ApplyOverlayMaterial(HoverOverlayMaterial);
		}
	}
}

bool AMainGameplayController::TrySelectUnitUnderCursor()
{
	if (bHasSelectedAUnit)
	{
		return false;
	}
	
	if (SelectedUnit == CursorUnit)
	{
		return false;
	}
	
	// Deselect if we have a current unit
	if (!CursorUnit)
	{	
		bHasSelectedAUnit = false;
		if (SelectedUnit)
		{
			SelectedUnit->DisableOverlayMaterial(HoverOverlayMaterial);
		}
		SelectedUnit = nullptr;
		return false;
	}
	// 1 is reserved Team Affiliation Index for the player
	if (CursorUnit->GetTeamAffiliationIndex() == 1)
	{
		SelectedUnit = CursorUnit;
		SelectedUnit->DisableOverlayMaterial(SelectOverlayMaterial);
		SelectedUnit->ApplyOverlayMaterial(SelectOverlayMaterial);
		SelectedUnit->ShowUnitAbility();
		SelectedUnit->GetObjectPool()->OnOwnerPooled.AddUniqueDynamic(this, &AMainGameplayController::SelectedUnitPooledWrapper);
		bHasSelectedAUnit = true;
		return true;
	}
	return false;
}

void AMainGameplayController::DeselectUnit()
{
	if (!SelectedUnit)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("DeSelected Unit!"))
	bHasSelectedAUnit = false;
	SelectedUnit->HideUnityAbility();
	SelectedUnit->DisableOverlayMaterial(SelectOverlayMaterial);
	SelectedUnit->GetObjectPool()->OnOwnerPooled.RemoveDynamic(this, &AMainGameplayController::SelectedUnitPooledWrapper);
	SelectedUnit = nullptr;
	if (ClickEventTarget)
	{
		ClickEventTarget = nullptr;
	}
}

void AMainGameplayController::SelectedUnitPooledWrapper(bool Status, UObject* PoolInstigator)
{
	DeselectUnit();
}

void AMainGameplayController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!SelectCursorActorObject && !bIsCursorVisible)
	{
		return;
	}
	
	SelectCursorActorObject->SetActorLocation(LocationUnderCursor);
}

void AMainGameplayController::RequestCursorIndicator_Implementation(UObject* RequestBy, bool ShowCursor)
{
	if (!SelectCursorActorObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Select Cusror Actor Object is Invalid."));
		return;
	}
	ClickEventTarget = RequestBy;
	bIsCursorVisible = ShowCursor;
	SelectCursorActorObject->SetActorHiddenInGame(!ShowCursor);
}

void AMainGameplayController::SendClickEvent()
{
	if (!ClickEventTarget)
	{
		return;
	}
	if (ClickEventTarget->Implements<UCursorReceiveHandlerInterface>())
	{
		ICursorReceiveHandlerInterface::Execute_NotifyCursorClicked
		(ClickEventTarget,
			HoveredActor ? HoveredActor : nullptr,
			LocationUnderCursor);
		return;
	}
}

void AMainGameplayController::SetAllowLaneIndicatorDetectionByCursor_Implementation(bool IsActive)
{
	bShouldSelectLaneIndicator = IsActive;
}
