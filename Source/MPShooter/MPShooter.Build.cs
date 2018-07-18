// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MPShooter : ModuleRules
{
	public MPShooter(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        //bEnforceIWYU = true;

        PublicDefinitions.Add("WITH_ADVANCED_SESSIONS=1");

        PrivateIncludePaths.AddRange(new string[] { "AdvancedSessions/Private"/*, "OnlineSubsystemSteam/Private"*/ });
        PublicIncludePaths.AddRange(new string[] { "AdvancedSessions/Public" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OnlineSubsystem", "CoreUObject", "OnlineSubsystemUtils", "Networking", "Sockets","Voice", "OnlineSubsystemSteam"});
        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "Sockets", "Networking", "OnlineSubsystemUtils","Voice", "Steamworks","OnlineSubsystemSteam"});
        PrivateDependencyModuleNames.Add("OnlineSubsystem");


    }
}
