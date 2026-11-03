#pragma once
#include "GameplayTagContainer.h"
#include "AttributeSet.h"
#include "UUnitData.generated.h"

class UNiagaraSystem;
class UUnitDetection;
class UAbility;
class AWeaponBase;

UCLASS(BlueprintType)
class AGEOFWAR_API UUnitData : public UPrimaryDataAsset
{
	
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	FName UnitName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	FGameplayTag UnitTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	TObjectPtr<UTexture2D> UnitImage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	bool CanAttackWhileMovement{false};
	
	// To Implement Later
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	bool CanAttackIfFriendlyUnitInTheWay{false};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Size")
	FVector Scale{1.f,1.f,1.f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cosmetic")
	TObjectPtr<USkeletalMesh> Mesh{nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimClass{nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AttackMontage{nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<UAbility> Attackbehaviour {nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<UAbility> Ability {nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UTexture2D> AbilityImage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TMap<FGameplayTag, TObjectPtr<UNiagaraSystem>> NiagaraSystems;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TMap<FGameplayTag, TObjectPtr<USoundBase>> AudioEffects;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAttributeSet AttributeSet {};
	
};