// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DisableAsyncComputeOnMapLoad : ModuleRules
{
	public DisableAsyncComputeOnMapLoad(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        PublicDependencyModuleNames.AddRange(new string[] { "Core" });
		PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine" });
		if (Target.bBuildEditor)
		{
            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
        }
	}
}
