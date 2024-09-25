// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DedicatedDemoClientTarget : TargetRules
{
	public DedicatedDemoClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        ExtraModuleNames.Add("DedicatedDemo");
		bWithLowLevelTestsOverride = false;
		if(Target.Platform == UnrealTargetPlatform.Mac)
		{
			bOverrideBuildEnvironment = true;
			AdditionalCompilerArguments = "-Wno-deprecated-pragma";  // you can add more separated with spaces here
		}
	}
}
