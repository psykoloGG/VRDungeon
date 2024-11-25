// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class VRDungeon : ModuleRules
{
	public VRDungeon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "XRBase", "VRGestures" });
	}
}
