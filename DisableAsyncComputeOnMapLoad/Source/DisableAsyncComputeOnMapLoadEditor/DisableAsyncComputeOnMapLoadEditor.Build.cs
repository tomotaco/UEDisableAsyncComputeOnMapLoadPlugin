// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DisableAsyncComputeOnMapLoadEditor : ModuleRules
{
	public DisableAsyncComputeOnMapLoadEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		PrivateIncludePathModuleNames.AddRange(new string[] { "Settings", "DisableAsyncComputeOnMapLoad" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core" });
        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "UnrealEd", "EditorSubsystem", "DisableAsyncComputeOnMapLoad" });
	}
}
