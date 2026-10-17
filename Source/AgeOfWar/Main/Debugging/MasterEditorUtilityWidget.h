// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"

#include "MasterEditorUtilityWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AGEOFWAR_API UMasterEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite)
	FName TabIcon = FName("Node.Debug");
	
	UPROPERTY(BlueprintReadWrite)
	FName TabStyle = FName("Node.Debug");
	
	// Don't know why unreal doesn't expose this already for generating new tabs
	UFUNCTION(BlueprintCallable)
	void ApplyIcon(FName TabID);
};
