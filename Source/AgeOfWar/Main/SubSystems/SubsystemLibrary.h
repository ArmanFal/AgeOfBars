#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"

namespace SubsystemsGetter
{
	template<typename T>
T* GetWorldSubsystem(const UObject* Context)
	{
		static_assert(TIsDerivedFrom<T, UWorldSubsystem>::IsDerived, "T must derive from UWorldSubsystem");

		if (!Context)
		{
			return nullptr;
		}
		UWorld* World{Context->GetWorld()};
		if (!World)
		{
			return nullptr;
		}
		return World->GetSubsystem<T>();
	}
	
	template<typename T>
	T* GetGameInstanceSubsystem(const UObject* Context)
	{
		static_assert(TIsDerivedFrom<T, UGameInstanceSubsystem>::IsDerived,
			"T must derive from UGameInstanceSubsystem");
		if (!Context)
		{
			return nullptr;
		}
		UWorld* World{Context->GetWorld()};
		if (!World)
		{
			return nullptr;
		}
		UGameInstance* GameInstance{World->GetGameInstance()};
		if (!GameInstance)
		{
			return nullptr;
		}
		return GameInstance->GetSubsystem<T>();
	}
}
