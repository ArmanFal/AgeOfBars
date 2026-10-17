// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h" 
#include "InputMappingContext.h"
#include "PlayerPerspectivePawn.generated.h"

class USpringArmComponent;

UCLASS()
class AGEOFWAR_API APlayerPerspectivePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPerspectivePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the pawn is possessed
	virtual void PossessedBy(AController* NewController) override;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MovingCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USplineComponent* Spline;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* PlayerRoot;

	UPROPERTY(BlueprintReadWrite)
	float CameraSpeed;

	UPROPERTY(BlueprintReadOnly)
	float SplineLength;

	float CurrentDistanceOnSpline;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Movement")
	bool ReverseCameraView;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Movement")
	bool ChangeRotationAlongSpline;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Movement")
	float CameraLocationInterpSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Movement")
	float CameraRotationInterpSpeed;


	float Direction;
public:	

	UFUNCTION(
		BlueprintCallable, 
		Category = "Camera Movement",
		meta=(ToolTip="Interpolates the camera to the desired location along Spline")
	)
	void MoveOnSpline();

	UFUNCTION(
		BlueprintCallable,
		Category = "Camera Movement",
		meta = (ToolTip = "Resets the Camera Location to the beginning of the Spline")
	)
	void ResetCameraLocationAlongSpline();


	UFUNCTION(BlueprintCallable, Category = "CameraMovement")
	void CameraRecieveinput(float CameraDirection);
};
