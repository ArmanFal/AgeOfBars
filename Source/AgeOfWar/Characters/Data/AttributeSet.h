#pragma once

#include "CoreMinimal.h"
#include "Characters/CustomComponents/Parent/AttributeBase.h"
#include "AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(Type, Name)\
	Type Get##Name() const			\
	{										\
	return Name;							\
	}										\
	void Set##Name(Type InValue, bool bApplyNewValueInstant)			\
	{										\
	if (bApplyNewValueInstant) { Name = InValue;} \
	OnAttributeChanged.Broadcast			\
	(InValue, Get##Name##Attribute());			\
	}										\
	static FAttributeBaseHandler Get##Name##Attribute() \
{\
	FProperty* Property = FindFProperty<FProperty>(FAttributeSet::StaticStruct(),\
		GET_MEMBER_NAME_CHECKED(FAttributeSet, Name));\
	return FAttributeBaseHandler(Property);\
}\

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, FAttributeBase, NewValue, FAttributeBaseHandler, Attribute);

USTRUCT(Blueprintable)
struct FAttributeSet
{
	
public:
	
	GENERATED_BODY()
	
	FOnAttributeChanged OnAttributeChanged;
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FAttributeBase Health{100.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FAttributeBase Damage{10.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FAttributeBase Speed{20.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FAttributeBase AttackRange{100.f};
	
public:
	
	ATTRIBUTE_ACCESSORS(FAttributeBase , Health)
	ATTRIBUTE_ACCESSORS(FAttributeBase , Damage)
	ATTRIBUTE_ACCESSORS(FAttributeBase , Speed)
	ATTRIBUTE_ACCESSORS(FAttributeBase , AttackRange)
};

#undef ATTRIBUTE_ACCESSORS