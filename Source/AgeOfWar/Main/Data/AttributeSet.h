#pragma once

#include "CoreMinimal.h"
#include "Main/CustomComponents/Parent/AttributeBase.h"
#include "AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(Type, Name)\
	Type Get##Name() const			\
	{										\
	return Name;							\
	}										\
	const Type& GetIdentity##Name() const			\
	{										\
		return Name;						\
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
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, FAttributeBase, NewValue, FAttributeBaseHandler, Attribute);

USTRUCT(BlueprintType)
struct FAttributeSet
{
	
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged;
	
	static void SetCurrentValueToBaseValue(FAttributeBase& Attribute)
	{
		Attribute.CurrentValue = Attribute.BaseValue;
	}

	void SetAllCurrentValuesToBaseValue()
	{
		for (TFieldIterator<FStructProperty> It(FAttributeSet::StaticStruct()); It; ++It)
		{
			FStructProperty* Property = *It;

			if (Property->Struct != FAttributeBase::StaticStruct())
			{
				continue;
			}

			FAttributeBase* FoundAttribute =
			Property->ContainerPtrToValuePtr<FAttributeBase>(this);

			if (FoundAttribute)
			{
				SetCurrentValueToBaseValue(*FoundAttribute);
			}
		}
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FAttributeBase Health{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FAttributeBase Damage{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FAttributeBase Speed{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FAttributeBase AttackRange{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FAttributeBase AttackSpeed{};
	
public:
	
	ATTRIBUTE_ACCESSORS(FAttributeBase , Health)
	ATTRIBUTE_ACCESSORS(FAttributeBase , Damage)
	ATTRIBUTE_ACCESSORS(FAttributeBase , Speed)
	ATTRIBUTE_ACCESSORS(FAttributeBase , AttackRange)
	ATTRIBUTE_ACCESSORS(FAttributeBase , AttackSpeed)
};

#undef ATTRIBUTE_ACCESSORS