// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LostArk : ModuleRules
{
	public LostArk(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "GameplayTasks" });
	}
}
