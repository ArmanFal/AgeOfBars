// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitDetectableInterface.h"
#include "CustomComponents//Combat and Attributes/Combat.h"
#include "CustomComponents/MinimalMovement.h"
#include "Data/UUnitData.h"
#include "CustomComponents/Combat and Attributes/AttributesComponent.h"
#include "CustomComponents/ObjectPool.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/Pawn.h"
#include "Managers/DamageableInterface.h"
#include "Managers/LaneManager.h"
#include "Unit.generated.h"

class UCombatLifeBar;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitStateChanged, EUnitDetectionState, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnitDied);

UCLASS()
class AGEOFWAR_API AUnit : public AActor,
	public IdamageableInterface,
	public IUnitDetectableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUnit();
	
	UPROPERTY(BlueprintAssignable, Category = "Detection")
	FOnUnitStateChanged OnUnitStateChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Death")
	FOnUnitDied OnUnitDied;
	
	UFUNCTION(BlueprintNativeEvent)
	void UnitSpawned();
	
	UFUNCTION(BlueprintCallable, Category = "Overrides")
	void SetTeamAffiliationIndex(int Index) { TeamAffiliationIndex = Index;};
	
	// UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	// void NewDetectionState(EUnitDetectionState State);
	
	UPROPERTY(BlueprintReadWrite)
	bool bAutomaticallyHandlePoolAfterDeath{true};
	
	UPROPERTY(BlueprintReadWrite)
	bool bCanDoAbility{true};
	
	UObjectPool* GetObjectPool() { return ObjectPoolComp; };

	UFUNCTION(BlueprintCallable, Category = "Overrides")
	bool ApplyDataToUnit(const UUnitData* Data);
	
	void ApplyOverlayMaterial(UMaterialInterface* Material);
	void DisableOverlayMaterial(UMaterialInterface* InputMaterial);
	void DisableAnyOverlayMaterial();
	
	void ShowUnitAbility();
	void HideUnityAbility();
	
	void SelectUnit();
	void DeselectUnit();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FAttributeSet GetUnitAttributes();
	
	UFUNCTION(BlueprintCallable)
	void SetOverrideDefaultDetectionBehaviour(bool StatusOverride);
	
	void AssignLaneManager(ALaneManager* AssignedLaneManager);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool bIsDetectionStopped{false};
	
	UFUNCTION()
	void HandleAttributeChangeDelegate(FAttributeBase Attribute, FAttributeBaseHandler AttributeHandle);
	
	void HandleMovementDelegate(float DeltaTime);
	EUnitDetectionState HandleUnitStateBasedOnDetectedUnits();
	
	EUnitDetectionState DetectionStateBehaviour();
	
	UFUNCTION()
	void HandlePoolStatus(bool IsInUse, UObject* PoolInstigator);
	
	UFUNCTION()
	void HandleDeath();
	
	UFUNCTION()
	bool DetectFrontUnit(float DeltaTime);
	
	UFUNCTION()
	void NotifyLaneManagerPowerChange();
	
	// Don't really need to add the AllowPrivateAccess but wouldn't wanna forget it in case we make the scope private
	// This section is for a variety of component which encapsulate the behaviour of the character
	
	// The Primary box collider used for all detection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> BoxCollision;
	
	//Movement component responsible for the pawn's movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMinimalMovement> MinimalMovementComp;
	
	//Attributes such as Health and Damage are stored in this component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAttributesComponent> AttributeComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UObjectPool> ObjectPoolComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCombat> CombatComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USkeletalMeshComponent> CharacterMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UWidgetComponent> AttributesBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UWidgetComponent> AbilityWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UWidgetInteractionComponent> InteractionWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	int32 TeamAffiliationIndex{0};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	ALaneManager* LaneManager;
	
	UPROPERTY(EditAnywhere, Category="Damage")
	TObjectPtr<UCombatLifeBar> LifeBarWidget;

	UPROPERTY(BlueprintReadOnly)
	TArray<FHitResult> FrontUnitHitResult;
	
	UPROPERTY(BlueprintReadOnly)
	AUnit* ClosestUnit;
	
	UPROPERTY(BlueprintReadOnly)
	FVector DetectionOffset{0.f,0.f,0.f};

	UPROPERTY(BlueprintReadOnly)
	float DetectionLength{100.f};
	
	UPROPERTY(BlueprintReadOnly)
	AUnit* DetectedFrontUnit{nullptr};
	
	UPROPERTY(BlueprintReadOnly)
	AActor* DetectedEnemyUnit{nullptr};
	
	UPROPERTY(BlueprintReadOnly)
	EAffiliationToSelf OtherDetectedUnitAffiliation{EAffiliationToSelf::UnknownAffiliation};
	
	FCollisionQueryParams CollisionQueryParams 
	{
		FName("UnitFrontTrace"),
		false, 
		this
	};
	
public:

	virtual bool DealDamage_Implementation(float Damage) override;
	
	
	virtual EAffiliationToSelf GetAffiliationOfTarget_Implementation(int index) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	FGameplayTag UnitTag;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ALaneManager* GetLaneManager();
	
	FVector ClosestImpactPoint;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FVector GetClosestEnemyImpactLocation() { return ClosestImpactPoint;} ;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LaneManager")
	int GetUnitIndexOnOwningLane();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	AActor* GetDetectedEnemyUnit() {return DetectedEnemyUnit; };
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components|Combat")
	int GetTeamAffiliationIndex() {return TeamAffiliationIndex;};
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:
	void UpdateUnitTag(FGameplayTag Tag);
};
