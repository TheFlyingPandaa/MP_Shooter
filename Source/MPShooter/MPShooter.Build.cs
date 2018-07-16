// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MPShooter : ModuleRules
{
	public MPShooter(ReadOnlyTargetRules Target) : base(Target)
	{
        bFasterWithoutUnity = true;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OnlineSubsystem", "OnlineSubsystemUtils", "Steamworks" });

        PrivateDependencyModuleNames.Add("OnlineSubsystem");

    }
}
