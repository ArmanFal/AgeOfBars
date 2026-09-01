#pragma once
#include "GameplayTagContainer.h"
#include "AttributeSet.h"
#include "UUnitData.generated.h"

class UAbility;

UCLASS(BlueprintType)
class AGEOFWAR_API UUnitData : public UPrimaryDataAsset
{
	
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	FGameplayTag UnitTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Size")
	FVector Scale{1.f,1.f,1.f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cosmetic")
	TObjectPtr<USkeletalMesh> Mesh{nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cosmetic")
	TSubclassOf<UAnimInstance> AnimClass{nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UAbility> Ability {nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "These are the Base values and cannot be changed in real time"))
	FAttributeSet BaseAttributes {BaseAttributes};
};