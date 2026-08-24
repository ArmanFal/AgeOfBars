// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPerspectivePawn.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <EnhancedInputSubsystems.h>


// Sets default values
APlayerPerspectivePawn::APlayerPerspectivePawn()
{
	PrimaryActorTick.bCanEverTick = false;

    // Camera Initilizations
    CurrentDistanceOnSpline = 0.f,
        Direction = 0.f,
        CameraSpeed = 300.f,
        CameraLocationInterpSpeed = 5.f,
        CameraRotationInterpSpeed = 5.f,
        ChangeRotationAlongSpline = false,
        ReverseCameraView = false;

	USceneComponent* SplineRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Player View Path"));
	SetRootComponent(SplineRoot);

	PlayerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Player Root"));

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    
    SpringArm->SetupAttachment(PlayerRoot);
	Spline->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

    SplineLength = Spline->GetSplineLength();
}

// Called when the game starts or when spawned
void APlayerPerspectivePawn::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerPerspectivePawn::PossessedBy(AController* NewController)
{

    Super::PossessedBy(NewController);

    if (APlayerController* PC = Cast<APlayerController>(NewController))
    {
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
                ResetCameraLocationAlongSpline();
            }
        }
    }
}

void APlayerPerspectivePawn::MoveOnSpline()
{
    if (!Spline || !PlayerRoot) return; // <- To Debug later

    // advance along spline
    CurrentDistanceOnSpline = FMath::Clamp(
        CurrentDistanceOnSpline + (Direction * CameraSpeed) * GetWorld()->GetDeltaSeconds(),
        0.f,
        Spline->GetSplineLength()
    );

    // location 
    FVector DesiredLocation = Spline->GetLocationAtDistanceAlongSpline(CurrentDistanceOnSpline, ESplineCoordinateSpace::World);
    FVector CurrentLocation = PlayerRoot->GetComponentLocation();
    


    FVector NewLocation = FMath::VInterpConstantTo(
    CurrentLocation,
    DesiredLocation,
    GetWorld()->GetDeltaSeconds(),
    CameraLocationInterpSpeed);
    
    PlayerRoot->AddWorldOffset(NewLocation - CurrentLocation);

    if (ChangeRotationAlongSpline)
    {
        FRotator CurrentRotation = PlayerRoot->GetComponentRotation();
        FRotator DesiredRotation = Spline->GetRotationAtDistanceAlongSpline(CurrentDistanceOnSpline, ESplineCoordinateSpace::World);
        PlayerRoot->SetWorldRotation(FMath::RInterpTo(CurrentRotation, DesiredRotation - CurrentRotation, GetWorld()->GetDeltaSeconds(), CameraRotationInterpSpeed));
    }
    
}

void APlayerPerspectivePawn::ResetCameraLocationAlongSpline()
{
    if (Spline && PlayerRoot)
    {
        CurrentDistanceOnSpline = 0.f;

        // Snap to spline start
        FVector StartLocation = Spline->GetLocationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World);
        PlayerRoot->SetWorldLocation(StartLocation);

        FRotator DesiredRotation = PlayerRoot->GetComponentRotation();

        if (ReverseCameraView)
        {
            DesiredRotation.Yaw += 180.f;
        }

        PlayerRoot->SetWorldRotation(DesiredRotation);
    }
}


void APlayerPerspectivePawn::CameraRecieveinput(float CameraDirection)
{
    Direction = -CameraDirection;
    MoveOnSpline();
}

