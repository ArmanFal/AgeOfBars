// Fill out your copyright notice in the Description page of Project Settings.
#include "Unit.h"

#include "Data/UUnitData.h"
#include "Managers/ObjectPoolHandlerInterface.h"


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
	
	if (ensure(ObjectPoolComp))
	{
		ObjectPoolComp->OnOwnerPooled.AddDynamic(this, &AUnit::HandlePoolStatus);
	}
	if (ensure(AttributeComp))
	{
		AttributeComp->OnDeath.AddDynamic(this, &AUnit::HandleDeath);
	}
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
	if (ensure(AttributeComp))
	{
		AttributeComp->OnAnyAttributeChanged.AddDynamic(this, &AUnit::NotifyLaneManagerPowerChange);
	}
}

void AUnit::HandleDeath()
{
	if (!bAutomaticallyHandlePoolAfterDeath)
	{
		return;
	}
	if (ensure(ObjectPoolComp))
	{
		IObjectPoolHandlerInterface::Execute_RequestReturnToPool(LaneManager, this);
	}
}

void AUnit::HandlePoolStatus(bool IsInUse, UObject* PoolInstigator)
{
	SetActorTickEnabled(IsInUse);
	if (!IsInUse)
	{
		ApplyDataToUnit(GetDefault<UUnitData>());
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
	if (ObjectPoolComp->GetIsInUse())
	{
		HandleMovementDelegate(DeltaTime);
	}
}

void AUnit::HandleMovementDelegate(float DeltaTime)
{
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

ALaneManager* AUnit::GetLaneManager()
{
	return LaneManager;
}

void AUnit::NotifyLaneManagerPowerChange()
{
	GetLaneManager()->ModifyLanePower(this);
}

