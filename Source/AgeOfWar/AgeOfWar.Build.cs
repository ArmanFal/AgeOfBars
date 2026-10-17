// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AgeOfWar : ModuleRules
{
	public AgeOfWar(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"Blutility",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"GamePlayTags",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AgeOfWar",
			"AgeOfWar/Variant_Platforming",
			"AgeOfWar/Variant_Platforming/Animation",
			"AgeOfWar/Variant_Combat",
			"AgeOfWar/Variant_Combat/AI",
			"AgeOfWar/Variant_Combat/Animation",
			"AgeOfWar/Variant_Combat/Gameplay",
			"AgeOfWar/Variant_Combat/Interfaces",
			"AgeOfWar/Variant_Combat/UI",
			"AgeOfWar/Variant_SideScrolling",
			"AgeOfWar/Variant_SideScrolling/AI",
			"AgeOfWar/Variant_SideScrolling/Gameplay",
			"AgeOfWar/Variant_SideScrolling/Interfaces",
			"AgeOfWar/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
