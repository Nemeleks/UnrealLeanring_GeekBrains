// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Lesson1 : ModuleRules
{
	public Lesson1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
