// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeBase.generated.h"

USTRUCT(BlueprintType)
struct AGEOFWAR_API FAttributeBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Value;
};

USTRUCT(BlueprintType)
struct AGEOFWAR_API FAttributeBaseHandler
{
	GENERATED_BODY()
	
	FAttributeBaseHandler() = default;
	
	FAttributeBaseHandler(FProperty* Property) : Property(Property) {}
	
	FProperty* Property = nullptr;
	
	FProperty* GetProperty() const
	{
		return Property;
	}
};
