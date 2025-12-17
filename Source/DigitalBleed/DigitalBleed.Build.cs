// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DigitalBleed : ModuleRules
{
	public DigitalBleed(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore",
			"MoviePlayer"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"DigitalBleed",
			"DigitalBleed/Variant_Platforming",
			"DigitalBleed/Variant_Platforming/Animation",
			"DigitalBleed/Variant_Combat",
			"DigitalBleed/Variant_Combat/AI",
			"DigitalBleed/Variant_Combat/Animation",
			"DigitalBleed/Variant_Combat/Gameplay",
			"DigitalBleed/Variant_Combat/Interfaces",
			"DigitalBleed/Variant_Combat/UI",
			"DigitalBleed/Variant_SideScrolling",
			"DigitalBleed/Variant_SideScrolling/AI",
			"DigitalBleed/Variant_SideScrolling/Gameplay",
			"DigitalBleed/Variant_SideScrolling/Interfaces",
			"DigitalBleed/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
