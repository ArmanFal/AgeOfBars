// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeBase.generated.h"

USTRUCT(BlueprintType)
struct AGEOFWAR_API FAttributeBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentValue{0.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float BaseValue{0.f};
};

USTRUCT(BlueprintType)
struct AGEOFWAR_API FAttributeBaseHandler
{
	GENERATED_BODY()
	
	FAttributeBaseHandler() = default;
	
	FAttributeBaseHandler(FProperty* Property) : Property(Property) {}
	
	bool operator==(const FAttributeBaseHandler& Other) const
	{
		return Property == Other.Property;
	}

	bool operator!=(const FAttributeBaseHandler& Other) const
	{
		return !(*this == Other);
	}
	
	FProperty* Property = nullptr;
	
	FProperty* GetProperty() const
	{
		return Property;
	}
};
