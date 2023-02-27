// Copyright Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class LicenseSpringPlugin : ModuleRules
{
	public LicenseSpringPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"LicenseSpringPlugin/Public",
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
				"LicenseSpringPlugin/Private",
				"LicenseSpringPlugin/ThirdParty/LicenseSpringPluginLibrary"
			}
		);
		
		string thirdPartyDir = Path.Combine(ModuleDirectory, "ThirdParty", "LicenseSpringPluginLibrary");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string platformDir = Path.Combine(thirdPartyDir, "x64", "Release");
			string binariesDir = Path.Combine(ModuleDirectory, "../../Binaries/Win64/");

			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(platformDir, "LicenseSpring.lib"));

			var libs = new List<string>() 
			{
				"LicenseSpring.dll",
				"libcrypto-1_1-x64.dll",
				"libcurl.dll",
				"libssl-1_1-x64.dll",
				"LSVMD.dll"
			};
			
			libs.ForEach(lib => PublicDelayLoadDLLs.Add(Path.Combine(platformDir, lib)));

			File.Copy(Path.Combine(platformDir, "LicenseSpring.dll"), Path.Combine(binariesDir, "LicenseSpring.dll"), true);
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			string platformDir = Path.Combine(thirdPartyDir, "Mac", "Release");

			var libs = new List<string>() 
			{ 
				"libcrypto.1.1.dylib", 
				"libcrypto.dylib", 
				"libcurl.4.dylib", 
				"libcurl.dylib", 
				"libLicenseSpring.dylib", 
				"libssl.1.1.dylib", 
				"libssl.dylib"
			};

			libs.ForEach(lib => PublicAdditionalLibraries.Add(Path.Combine(platformDir, lib)));
			libs.ForEach(lib => PublicDelayLoadDLLs.Add(Path.Combine(platformDir, lib)));
			libs.ForEach(lib => RuntimeDependencies.Add(Path.Combine(platformDir, lib)));
		}

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// "LicenseSpringPluginLibrary",
				"Projects"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		
		
	}
}
