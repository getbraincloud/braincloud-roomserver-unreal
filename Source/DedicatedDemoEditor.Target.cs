// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DedicatedDemoEditorTarget : TargetRules
{
	public DedicatedDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("DedicatedDemo");
		bWithLowLevelTestsOverride = false;
		bOverrideBuildEnvironment = true;
		if(Target.Platform == UnrealTargetPlatform.Mac)
		{
			AdditionalCompilerArguments = "-Wno-deprecated-pragma";  // you can add more separated with spaces here
		}
	}
}
