// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CursorHandlerInterface.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Main/Unit.h"
#include "MainGameplayController.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFWAR_API AMainGameplayController : public APlayerController, public ICursorHandlerInterface
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
public:
	
	virtual void RequestCursorIndicator_Implementation(UObject* RequestBy, bool ShowCursor) override;
	virtual void RequestCursorDeselect_Implementation() override { DeselectUnit(); } ;
	virtual void SetAllowLaneIndicatorDetectionByCursor_Implementation(bool IsActive) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input | Cursor")
	TSubclassOf<AActor> SelectCursorActorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input | Cursor")
	FVector CursorActorLockOffset;
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DetectUnderCursor(float DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool TrySelectUnitUnderCursor();
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DeselectUnit();
	
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	bool bHasSelectedAUnit;
	
	UPROPERTY(BlueprintReadOnly)
	FHitResult CursorHitResult;
	
	UPROPERTY(BlueprintReadOnly)
	AUnit* CursorUnit;
	
	UPROPERTY(BlueprintReadOnly)
	AUnit* SelectedUnit;
	
	UPROPERTY(BlueprintReadOnly)
	AActor* HoveredActor;
	
	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* HoverOverlayMaterial{nullptr};
	
	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* SelectOverlayMaterial;
	
	UPROPERTY(BlueprintReadWrite)
	int MyTeamAffiliation{1};
	
	UFUNCTION(BlueprintCallable, Category = "Input | Cursor")
	AActor* GetCursorActorInLevel() {return SelectCursorActorObject;};
	
	UFUNCTION(BlueprintCallable, Category = "Input | Cursor")
	void SendClickEvent();

	UPROPERTY()
	UObject* ClickEventTarget{nullptr};

	
private:
	
	bool bShouldSelectLaneIndicator{false};
	
	UPROPERTY()
	TObjectPtr<AActor> SelectCursorActorObject;
	
	UFUNCTION()
	void SelectedUnitPooledWrapper(bool Status, UObject* PoolInstigator);

	FVector LocationUnderCursor{FVector::ZeroVector};
	
	bool bIsCursorVisible{false};
};
