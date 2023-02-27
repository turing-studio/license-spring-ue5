// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnrealBuildTool;

public class LicenseSpringPluginLibrary : ModuleRules
{
	public LicenseSpringPluginLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		PublicIncludePaths.AddRange(
			new string[] {
                "LicenseSpringPlugin/Public",
                "LicenseSpringPlugin/ThirdParty/LicenseSpringPluginLibrary"
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
                "LicenseSpring/Private"
			}
		);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string thirdPartyDir = Path.Combine(ModuleDirectory, "x64", "Release");

			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(thirdPartyDir, "LicenseSpring.lib"));

			var libs = new List<string>() 
			{
				"LicenseSpring.dll",
				"libcrypto-1_1-x64.dll",
				"libcurl.dll",
				"libssl-1_1-x64.dll",
				"LSVMD.dll"
			};

			libs.ForEach(lib => PublicDelayLoadDLLs.Add(Path.Combine(thirdPartyDir, lib)));
			libs.ForEach(lib => RuntimeDependencies.Add(Path.Combine(thirdPartyDir, lib)));

			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/LicenseSpringPluginLibrary/Win64/ExampleLibrary.dll");
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
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

			string thirdPartyDir = Path.Combine(ModuleDirectory, "Mac", "Release");
			libs.ForEach(lib => PublicDelayLoadDLLs.Add(Path.Combine(thirdPartyDir, lib)));
			libs.ForEach(lib => RuntimeDependencies.Add(Path.Combine(thirdPartyDir, lib)));
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			string ExampleSoPath = Path.Combine("$(PluginDir)", "Binaries", "ThirdParty", "LicenseSpringPluginLibrary", "Linux", "x86_64-unknown-linux-gnu", "libExampleLibrary.so");
			PublicAdditionalLibraries.Add(ExampleSoPath);
			PublicDelayLoadDLLs.Add(ExampleSoPath);
			RuntimeDependencies.Add(ExampleSoPath);
		}
	}
}
