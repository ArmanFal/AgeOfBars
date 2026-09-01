// Fill out your copyright notice in the Description page of Project Settings.
#include "Unit.h"

#include "Ability.h"
#include "Data/UUnitData.h"
#include "Managers/ObjectPoolHandlerInterface.h"


// Sets default values
AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	MinimalMovement = CreateDefaultSubobject<UMinimalMovement>(TEXT("MinimalMovementComponent"));
	CombatComp = CreateDefaultSubobject<UCombat>(TEXT("CombatComponent"));
	AttributeComponent = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
	ObjectPool = CreateDefaultSubobject<UObjectPool>(TEXT("Object Pool Component"));
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	AbilityWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("AbilityIndicatorWidget"));
	AttributesBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("AttributeBar"));
	InteractionWidget = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("InteractionWidget"));
	
	RootComponent = BoxCollision;
	CharacterMesh->SetupAttachment(RootComponent);
	AbilityWidget->SetupAttachment(RootComponent);
	AttributesBar->SetupAttachment(RootComponent);
	InteractionWidget->SetupAttachment(RootComponent);
	
	if (ensure(ObjectPool))
	{
		ObjectPool->OnOwnerPooled.AddDynamic(this, &AUnit::HandlePoolStatus);
	}
	if (ensure(AttributeComponent))
	{
		AttributeComponent->OnDeath.AddDynamic(this, &AUnit::HandleDeath);
	}
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
}

void AUnit::HandleDeath()
{
	if (!bAutomaticallyHandlePoolAfterDeath)
	{
		return;
	}
	if (ensure(ObjectPool))
	{
		IObjectPoolHandlerInterface::Execute_RequestReturnToPool(ObjectPool, this);
	}
}

void AUnit::HandlePoolStatus(bool IsInUse, UObject* PoolInstigator)
{
	if (IsInUse)
	{
		MinimalMovement->BeginMovement();
		return;	
	}
	ApplyDataToUnit(GetDefault<UUnitData>());
	MinimalMovement->StopMovement();
}

bool AUnit::ApplyDataToUnit(const UUnitData* Data) 
{
	if (!Data)
	{
		return false;
	}
	CharacterMesh->SetWorldScale3D(Data->Scale);
	CharacterMesh->SetSkeletalMesh(Data->Mesh);
	CharacterMesh->SetAnimInstanceClass(Data->AnimClass);
	AttributeComponent->SetBaseAttributes(Data->BaseAttributes);
	CombatComp->AssignAbility(Data->Ability);
	UpdateUnitTag(Data->UnitTag);
	return true;
}

void AUnit::UpdateUnitTag(FGameplayTag Tag) 
{
	UnitTag = Tag;
}


// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DetectForward();
	
}

bool AUnit::DetectForward()
{
	// GetWorld()->LineTraceSingleByObjectType();
	return false;
}

// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

