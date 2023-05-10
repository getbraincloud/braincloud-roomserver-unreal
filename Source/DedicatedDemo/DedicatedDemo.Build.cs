// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DedicatedDemo : ModuleRules
{
	public DedicatedDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });

        PrivateDependencyModuleNames.AddRange(new string[] { "HTTP", "OnlineSubsystem", "Json", "JsonUtilities" });
    }
}
