// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class AzureKinectEditor : ModuleRules
{
	public AzureKinectEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		//OverridePackageType = PackageOverrideType.GameUncookedOnly;
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.AddRange(
			new string[]
			{
				"AzureKinectEditor/Private",
				"AzureKinectEditor/Private/AnimNodes",
				"AzureKinectEditor/Private/AssetTools",
				"AzureKinectEditor/Private/Customizations",
				"AzureKinectEditor/Private/Factories",
			});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",	
				"SlateCore",
				"PropertyEditor",
				"InputCore",
				"AzureKinect",
				"UnrealEd",
				"BlueprintGraph",
				"AnimGraph",
			});

	}
}
