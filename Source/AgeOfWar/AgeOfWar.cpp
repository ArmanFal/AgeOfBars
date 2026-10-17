// Copyright Epic Games, Inc. All Rights Reserved.

#include "AgeOfWar.h"
#include "Modules/ModuleManager.h"

#if WITH_EDITOR
#include "Engine/Texture2D.h"
#include "UObject/UObjectGlobals.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#endif

class FAgeOfBars : public FDefaultGameModuleImpl
{
public:

    virtual void StartupModule() override
    {
        FDefaultGameModuleImpl::StartupModule();

#if WITH_EDITOR

        // Load the existing Texture2D asset
        IconTexture = LoadObject<UTexture2D>(
            nullptr,
            TEXT("/Game/AgeOfWarNewSet/Debugging/DebuggingIcon.DebuggingIcon")
        );

        if (!IconTexture)
        {
            return;
        }

        // Prevent garbage collection while Slate uses the texture
        IconTexture->AddToRoot();

        // Create a named Slate style set
        PoolingStyle = MakeShared<FSlateStyleSet>(
            FName(TEXT("CustomStyle"))
        );

        // Create a Slate brush using the Texture2D
        FSlateBrush* IconBrush = new FSlateBrush();

        IconBrush->SetResourceObject(IconTexture);
        IconBrush->ImageSize = FVector2D(32.0f, 32.0f);
        IconBrush->DrawAs = ESlateBrushDrawType::Image;
        IconBrush->TintColor = FSlateColor(FLinearColor::White);

        // Register the brush under its style name
        PoolingStyle->Set(
            FName(TEXT("Pooling.Icon")),
            IconBrush
        );

        // Register the style with Unreal's Slate system
        FSlateStyleRegistry::RegisterSlateStyle(
            *PoolingStyle
        );

#endif
    }

    virtual void ShutdownModule() override
    {
#if WITH_EDITOR

        // Unregister the Slate style
        if (PoolingStyle.IsValid())
        {
            FSlateStyleRegistry::UnRegisterSlateStyle(
                *PoolingStyle
            );

            PoolingStyle.Reset();
        }

        // Allow Unreal to garbage collect the texture
        if (IconTexture)
        {
            IconTexture = nullptr;
        }

#endif

        FDefaultGameModuleImpl::ShutdownModule();
    }

private:

#if WITH_EDITOR

    TSharedPtr<FSlateStyleSet> PoolingStyle;

    UTexture2D* IconTexture = nullptr;

#endif
};

IMPLEMENT_PRIMARY_GAME_MODULE( FAgeOfBars, AgeOfWar, "AgeOfWar" );

DEFINE_LOG_CATEGORY(LogAgeOfWar)