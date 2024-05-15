// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DedicatedDemoTarget : TargetRules
{
	public DedicatedDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("DedicatedDemo");
		bWithLowLevelTestsOverride = false;
		if(Target.Platform == UnrealTargetPlatform.Mac)
		{
			bOverrideBuildEnvironment = true;
			AdditionalCompilerArguments = "-Wno-deprecated-pragma";  // you can add more separated with spaces here
		}
	}
}
