// Fill out your copyright notice in the Description page of Project Settings.
#include "Unit.h"

#include "CombatLifeBar.h"
#include "SNegativeActionButton.h"
#include "Data/UUnitData.h"
#include "Managers/ObjectPoolHandlerInterface.h"
#include "SubSystems/SubsystemLibrary.h"
#include "SubSystems/PoolSingleton/UnitPoolHandler.h"


// Sets default values
AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	MinimalMovementComp = CreateDefaultSubobject<UMinimalMovement>(TEXT("MinimalMovementComponent"));
	CombatComp = CreateDefaultSubobject<UCombat>(TEXT("CombatComponent"));
	AttributeComp = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
	ObjectPoolComp = CreateDefaultSubobject<UObjectPool>(TEXT("Object Pool Component"));
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	AbilityWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("AbilityIndicatorWidget"));
	AttributesBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("AttributeBar"));
	InteractionWidget = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("InteractionWidget"));
	
	RootComponent = BoxCollision;
	CharacterMesh->SetupAttachment(RootComponent);
	AbilityWidget->SetupAttachment(RootComponent);
	AttributesBar->SetupAttachment(RootComponent);
	InteractionWidget->SetupAttachment(RootComponent);
	
	if (AbilityWidget)
	{
		AbilityWidget->SetHiddenInGame(true);
	}
	
	if (ensure(ObjectPoolComp))
	{
		ObjectPoolComp->OnOwnerPooled.AddDynamic(this, &AUnit::HandlePoolStatus);
	}
	if (ensure(AttributeComp))
	{
		AttributeComp->OnDeath.AddDynamic(this, &AUnit::HandleDeath);
	}
	
	AttributesBar->SetWidgetClass(LifeBarWidget.GetClass());
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
	LifeBarWidget = Cast<UCombatLifeBar>(AttributesBar->GetUserWidgetObject());
	check(LifeBarWidget);
	LifeBarWidget->SetLifePercentage(0.f);	
	if (ensure(AttributeComp))
	{
		AttributeComp->OnAttributeChangedOnAttributeComp.AddDynamic(this, &AUnit::HandleAttributeChangeDelegate);
	}
}

void AUnit::HandleAttributeChangeDelegate(FAttributeBase Attribute, FAttributeBaseHandler AttributeHandle)
{
	/*
	 * Can use this part to handle non gameplay effects like UI or edge cases.
	 */
	if (AttributeHandle == FAttributeSet::GetHealthAttribute())
	{
		check(LifeBarWidget);
		float HealthPercent = 0;
		if (!FMath::IsNearlyZero(Attribute.BaseValue))
		{
			HealthPercent = Attribute.CurrentValue / Attribute.BaseValue;
		}
		LifeBarWidget->SetLifePercentage(HealthPercent);	
	}
	NotifyLaneManagerPowerChange();
}

void AUnit::HandleDeath()
{
	if (!bAutomaticallyHandlePoolAfterDeath)
	{
		return;
	}
	if (ensure(ObjectPoolComp))
	{
		UUnitPoolHandler* PoolHandler = SubsystemsGetter::GetWorldSubsystem<UUnitPoolHandler>(this);
		IObjectPoolHandlerInterface::Execute_RequestReturnToPool(PoolHandler, this, this);
	}
}

void AUnit::HandlePoolStatus(bool IsInUse, UObject* PoolInstigator)
{
	SetActorTickEnabled(IsInUse);
	check(LifeBarWidget)
	if (IsInUse)
	{
		// Temporarily we set the health to always be one when we pool
		LifeBarWidget->SetLifePercentage(1.f);
	}
	if (!IsInUse)
	{
		ApplyDataToUnit(GetDefault<UUnitData>());
		LaneManager = nullptr;
		TeamAffiliationIndex = 0;
		MinimalMovementComp->OnTickMovementDelegate.Unbind();
		MinimalMovementComp->StopMovement();
		SetActorTickEnabled(false);
	}
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
	AttributeComp->SetAttribute(Data->AttributeSet);
	CombatComp->AssignUnitAbility(Data->Ability);
	CombatComp->AssignAttackBehaviour(Data->Attackbehaviour);
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
	if (ObjectPoolComp->GetIsInUse())
	{
		HandleMovementDelegate(DeltaTime);
	}
}

void AUnit::HandleMovementDelegate(float DeltaTime)
{
	if (bIsDetectionStopped)
	{
		return;
	}
	if (DetectFrontUnit(DeltaTime))
	{
		const EUnitDetectionState State = HandleUnitStateBasedOnDetectedUnits();
		OnUnitStateChanged.Broadcast(State);
	}
	else
	{
		MinimalMovementComp->BeginMovement();
	}
}

EUnitDetectionState AUnit::HandleUnitStateBasedOnDetectedUnits()
{
	bool bFoundFriendly = false;
	bool bFoundHigherIndexFriendly = false;
	bool bHasFoundEnemy = false;
	
	float UnitDistance = TNumericLimits<float>::Max();;
	AUnit* ClosestUnitCandidate = nullptr;
	AUnit* ClosetEnemyUnitCandidate = nullptr;
	
	if (!GetLaneManager())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has No Valid Lane Manager."), *GetName());
		return EUnitDetectionState::UnknownState;
	}
	if (GetLaneManager()->GetUnitIndex(this) == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has No Valid UnitIndex Registered at LaneManager."), *GetName());
		return EUnitDetectionState::UnknownState;
	}
	
	const int ThisIndex = GetLaneManager()->GetUnitIndex(this);
	
	if (!GetLaneManager())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has No Valid Lane Manager."), *GetName());
		return EUnitDetectionState::UnknownState;
	}

		for (int i = 0; i < FrontUnitHitResult.Num(); i ++)
		{
			FHitResult& Hits = FrontUnitHitResult[i];
			AUnit * Unit = Cast<AUnit>(Hits.GetActor());
			
			if (!Unit)
			{
				OtherDetectedUnitAffiliation = EUnitAffiliation::UnknownAffiliation;
				continue;
			}
			
			if (!Unit->GetLaneManager())
			{
				UE_LOG(LogTemp, Warning, TEXT("Detected Actor Has An Invalid Lane"));
				return EUnitDetectionState::UnknownState;
			}
			
			EUnitAffiliation UnitAffiliation = CombatComp->GetDetectedUnitAffiliation(this, Unit);
			
			float IterationDist = FVector::Dist(GetActorLocation(), Unit->GetActorLocation());
			
			if (IterationDist < UnitDistance)
			{
				UnitDistance = IterationDist;
				ClosestUnitCandidate = Unit;
			}
			
			const int OtherIndex = GetLaneManager()->GetUnitIndex(Unit);
			
			switch (UnitAffiliation)
			{
			default: UE_LOG(LogTemp, Warning, TEXT("Handling unit state returned default")); break;
				
			case EUnitAffiliation::Enemy :
				{
					const int EnemyIndex = Unit->GetLaneManager()->GetUnitIndex(Unit);
					bHasFoundEnemy = true;
					if (ClosetEnemyUnitCandidate)
					{
						if (EnemyIndex > OtherIndex)
						{
							ClosetEnemyUnitCandidate = Unit;
						}
					}
					else
					{
						ClosetEnemyUnitCandidate =  Unit;
					}
					break;
				} 
				
			case EUnitAffiliation::UnknownAffiliation:
					{
						UE_LOG(LogTemp, Warning, TEXT("Unknown Affiliation"));
					break;
					}
				
			case EUnitAffiliation::Friendly:
				{
					if (ThisIndex < OtherIndex)
					{
							// We are behind the detected unit.
							bFoundHigherIndexFriendly = true;
					}

						// We are ahead of the detected unit.
						bFoundFriendly = true;
						break;
				}
				
			}
				
		}
	
	DetectedFrontUnit = ClosestUnitCandidate;
	if (DetectedFrontUnit)
	{
		OtherDetectedUnitAffiliation = 
			CombatComp->GetDetectedUnitAffiliation(this, DetectedFrontUnit);
	}
	
	if (bFoundFriendly)
	{
		if (bFoundHigherIndexFriendly)
		{
			if (bHasFoundEnemy)
			{
				DetectedEnemyUnit = ClosetEnemyUnitCandidate;
				return EUnitDetectionState::EnemyDetectedWhileBehindFriendly;
			}
			return EUnitDetectionState::FriendlyOnlyDetected;
		}
		return EUnitDetectionState::NoBlockingUnitDetected;
	} 
	if (bHasFoundEnemy)
	{
		DetectedEnemyUnit = ClosetEnemyUnitCandidate;
		return EUnitDetectionState::EnemiesOnlyDetected;
	}

	OtherDetectedUnitAffiliation =
		EUnitAffiliation::UnknownAffiliation;

	return EUnitDetectionState::UnknownState;
}

bool AUnit::DetectFrontUnit(float DeltaTime)
{
	FVector BeginPoint = GetActorLocation() + DetectionOffset;
	FVector EndPoint = (GetActorLocation() + 
		(GetActorForwardVector() * AttributeComp->GetAttributeSet()->GetAttackRange().CurrentValue)) + DetectionOffset;
	
	GetWorld()->LineTraceMultiByChannel(
	FrontUnitHitResult,
	BeginPoint,
	EndPoint,
	ECC_Visibility,
	CollisionQueryParams
);
	// We have Detected any Unit in front of us
	bool bHit = !FrontUnitHitResult.IsEmpty();
	
	DrawDebugLine(GetWorld(), BeginPoint, EndPoint, bHit ? FColor::Red : FColor::Green,
		false, DeltaTime);
	
	return bHit;
}


void AUnit::AssignLaneManager(ALaneManager* AssignedLaneManager)
{
	if (AssignedLaneManager)
	{
		TeamAffiliationIndex = AssignedLaneManager->GetTeamAffiliation();
		LaneManager = AssignedLaneManager;
		return;
	}
	Destroy();
}

void AUnit::SetOverrideDefaultDetectionBehaviour(bool StatusOverride)
{
	bIsDetectionStopped = StatusOverride;
}

bool AUnit::DealDamage_Implementation(float Damage)
{
	if (!AttributeComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has No Attribute Component"), *GetName())
	}
	
	if (!CombatComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has No Attribute Component"), *GetName())
	}
	float FinalDamage = AttributeComp->ApplyDamage(Damage);
	return FMath::Abs(FinalDamage) > 0.f;
}

const AUnit* AUnit::GetClosestUnitInLane()
{
	AUnit* thisClosestUnit = nullptr;
	float Distance = TNumericLimits<float>::Max();
	
	for (int i = 0; i < FrontUnitHitResult.Num(); i++)
	{
		FVector ThisUnitLocation = GetActorLocation();
		AUnit* UnitCandidate = Cast<AUnit>(FrontUnitHitResult[i].GetActor());
		
		if (!UnitCandidate)
		{
			continue;
		}
		
		FVector UnitCandidateLocation = UnitCandidate->GetActorLocation();
		
		const float DistanceBetweenUnits = FVector::Dist(UnitCandidateLocation, ThisUnitLocation);
		
		if (DistanceBetweenUnits < Distance)
		{
			Distance = DistanceBetweenUnits;
			thisClosestUnit = UnitCandidate;
		}
	}
	return thisClosestUnit;
}

ALaneManager* AUnit::GetLaneManager()
{
	return LaneManager;
}

int AUnit::GetUnitIndexOnOwningLane() 
{
	if (GetLaneManager())
	{
		return GetLaneManager()->GetUnitIndex(this);
	}
	UE_LOG(LogTemp, Warning, TEXT("Invalid Lane Manager"))
	return 0;
}

FAttributeSet AUnit::GetUnitAttributes()
{
	return AttributeComp->GetAttributeSetCopy();
}

void AUnit::NotifyLaneManagerPowerChange()
{
	if (GetLaneManager())
	{
		GetLaneManager()->ModifyLanePower(this);
	}
}

void AUnit::ApplyOverlayMaterial(UMaterialInterface* Material)
{
	if (!Material)
	{
		UE_LOG(LogTemp, Warning, TEXT("Material Applied to unit is invalid."))
		return;
	}
	CharacterMesh->SetOverlayMaterial(Material);
}

void AUnit::DisableOverlayMaterial(UMaterialInterface* InputMaterial)
{
	if (CharacterMesh->GetOverlayMaterial() == InputMaterial)
	{
		CharacterMesh->SetOverlayMaterial(nullptr);
	}
}

void AUnit::SelectUnit()
{
	if (AbilityWidget)
	{
		AbilityWidget->SetHiddenInGame(false);
	}
}

void AUnit::DeselectUnit()
{
	if (AbilityWidget)
	{
		AbilityWidget->SetHiddenInGame(true);
	}
}

void AUnit::DisableAnyOverlayMaterial()
{
	CharacterMesh->SetOverlayMaterial(nullptr);
}

void AUnit::ShowUnitAbility()
{
	if (!AbilityWidget)
	{
		return;
	}
	AbilityWidget->SetHiddenInGame(false);
}

void AUnit::HideUnityAbility()
{
	if (!AbilityWidget)
	{
		return;
	}
	AbilityWidget->SetHiddenInGame(true);
}


