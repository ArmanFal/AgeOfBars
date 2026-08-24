#pragma once

#include "CoreMinimal.h"
#include "Attributes.generated.h"

#define ATTRIBUTE_ACCESSORS(Type, Name)\
	Type Get##Name() const			\
	{										\
	return Name;							\
	}										\
	void Set##Name(Type InValue)			\
	{										\
	Name = InValue;							\
	}										

USTRUCT(Blueprintable)
struct FAttributes
{
	
public:
	
	GENERATED_BODY()
	
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth{100.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxHealth{100.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Damage{10.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Speed{20.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AttackRange{100.f};
	
public:
	
	ATTRIBUTE_ACCESSORS(float, CurrentHealth)
	ATTRIBUTE_ACCESSORS(float, MaxHealth)
	ATTRIBUTE_ACCESSORS(float, Damage)
	ATTRIBUTE_ACCESSORS(float, Speed)
	ATTRIBUTE_ACCESSORS(float, AttackRange)
};

#undef ATTRIBUTE_ACCESSORS