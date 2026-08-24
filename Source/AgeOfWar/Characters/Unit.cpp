// Fill out your copyright notice in the Description page of Project Settings.
#include "Unit.h"


// Sets default values
AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	MinimalMovement = CreateDefaultSubobject<UMinimalMovement>(TEXT("MinimalMovementComponent"));
	CombatComp = CreateDefaultSubobject<UCombat>(TEXT("CombatComponent"));
	Attributes = CreateDefaultSubobject<UStatAttributes>(TEXT("Attributes"));
	ObjectPool = CreateDefaultSubobject<UObjectPool>(TEXT("Object Pool Component"));
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	AbilityWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("AbilityIndicatorWidget"));
	AttributesBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("AttributeBar"));
	
	RootComponent = BoxCollision;
	CharacterMesh->SetupAttachment(RootComponent);
	AbilityWidget->SetupAttachment(RootComponent);
	AttributesBar->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

