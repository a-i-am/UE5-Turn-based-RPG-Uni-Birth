// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UniBirth : ModuleRules
{
	public UniBirth(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "UniBirth" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject",
            "Engine", "InputCore", "EnhancedInput",
            "UMG", "Niagara", "NavigationSystem", "AIModule",
            "GameplayTags",
            "OnlineSubsystem", "OnlineSubsystemUtils",
            "MoviePlayer", "Slate", "SlateCore", "LevelSequence", "MovieScene", "CinematicCamera"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
	

	}
}
