// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
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
				// ... add public include paths required here ...
			}
			);

		PrivateIncludePaths.AddRange(
			new string[] {
                "LicenseSpring/Private"
				// ... add other private include paths required here ...
			}
			);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "ExampleLibrary.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("ExampleLibrary.dll");

			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/LicenseSpringPluginLibrary/Win64/ExampleLibrary.dll");
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
			string thirdPartyDir = Path.Combine(ModuleDirectory, "Mac", "Release");
            PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libcrypto.1.1.dylib"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libcrypto.dylib"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libcurl.4.dylib"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libcurl.dylib"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libLicenseSpring.dylib"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libssl.1.1.dylib"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libssl.dylib"));
            //RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/LicenseSpringPluginLibrary/Mac/Release/libcrypto.1.1.dylib");
			//RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/LicenseSpringPluginLibrary/Mac/Release/libcrypto.dylib");
			//RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/LicenseSpringPluginLibrary/Mac/Release/libcurl.4.dylib");
			//RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/LicenseSpringPluginLibrary/Mac/Release/libcurl.dylib");
			// RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/LicenseSpringPluginLibrary/Mac/Release/libLicenseSpring.dylib");
			//RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/LicenseSpringPluginLibrary/Mac/Release/libssl.1.1.dylib");
			//RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/LicenseSpringPluginLibrary/Mac/Release/libssl.dylib");

			// string binariesDir = Path.Combine(ModuleDirectory, "../../Binaries/Mac/");
        	// Directory.CreateDirectory(binariesDir);
			// File.Copy(Path.Combine(thirdPartyDir, "libcrypto.1.1.dll"), Path.Combine(binariesDir, "libcrypto.1.1.dll"), true);
			// File.Copy(Path.Combine(thirdPartyDir, "libcrypto.dll"), Path.Combine(binariesDir, "libcrypto.dll"), true);
			// File.Copy(Path.Combine(thirdPartyDir, "libcurl.4.dll"), Path.Combine(binariesDir, "libcurl.4.dll"), true);
			// File.Copy(Path.Combine(thirdPartyDir, "libcurl.dll"), Path.Combine(binariesDir, "libcurl.dll"), true);
			// File.Copy(Path.Combine(thirdPartyDir, "libLicenseSpring.dll"), Path.Combine(binariesDir, "libLicenseSpring.dll"), true);
			// File.Copy(Path.Combine(thirdPartyDir, "libssl.1.1.dll"), Path.Combine(binariesDir, "libssl.1.1.dll"), true);
			// File.Copy(Path.Combine(thirdPartyDir, "libssl.dll"), Path.Combine(binariesDir, "libssl.dll"), true);

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
