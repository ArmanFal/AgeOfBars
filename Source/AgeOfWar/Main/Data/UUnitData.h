#pragma once
#include "GameplayTagContainer.h"
#include "AttributeSet.h"
#include "UUnitData.generated.h"

class UUnitDetection;
class UAbility;
class AWeaponBase;

UCLASS(BlueprintType)
class AGEOFWAR_API UUnitData : public UPrimaryDataAsset
{
	
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	FGameplayTag UnitTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "These are the Base values and cannot be changed in real time"))
	bool CanAttackWhileMovement{false};
	
	// To Implement Later
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "These are the Base values and cannot be changed in real time"))
	bool CanAttackIfFriendlyUnitInTheWay{false};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Size")
	FVector Scale{1.f,1.f,1.f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cosmetic")
	TObjectPtr<USkeletalMesh> Mesh{nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimClass{nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AttackMontage{nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UAbility> Ability {nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAttributeSet AttributeSet {};
};