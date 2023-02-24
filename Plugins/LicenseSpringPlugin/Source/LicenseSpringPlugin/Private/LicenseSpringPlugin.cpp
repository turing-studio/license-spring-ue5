// Copyright Epic Games, Inc. All Rights Reserved.

#include "LicenseSpringPlugin.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
//#include "LicenseSpringPluginLibrary/ExampleLibrary.h"
#include "LicenseSpringPluginLibrary/LicenseManager.h"

#define LOCTEXT_NAMESPACE "FLicenseSpringPluginModule"


void FLicenseSpringPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("LicenseSpringPlugin")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/LicenseSpringPluginLibrary/Win64/ExampleLibrary.dll"));
#elif PLATFORM_MAC
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/LicenseSpringPluginLibrary/Mac/Release/libExampleLibrary.dylib"));
#elif PLATFORM_LINUX
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/LicenseSpringPluginLibrary/Linux/x86_64-unknown-linux-gnu/libExampleLibrary.so"));
#endif // PLATFORM_WINDOWS

	ExampleLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (ExampleLibraryHandle)
	{

		UE_LOG(LogTemp, Log, TEXT("Library handle aquired"));
		// Call the test function in the third party library that opens a message box
//		ExampleLibraryFunction();
        LicenseSpring::Configuration::ptr_t pconf;
        try
        {
            // Values are copied from sample project
            pconf = LicenseSpring::Configuration::Create( "afce72fb-9fba-406e-8d19-ffde5b0a7cad",
                "Qc8EdU7DY-gMI87-JMueZWXdtJ0Ek_hS6dGC_SwusO8", "DP", "Cpp Sample", "1.1" );
        }
        catch( const std::exception& exc )
        {
//            LogException( baseDir, exc );
            FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "LicenseSpring::Configuration::Create failed"));
        }
        
        auto licenseManager = LicenseSpring::LicenseManager::create(pconf);

		UE_LOG(LogTemp, Log, TEXT("Get license"));
		try 
		{
			auto license = licenseManager->getCurrentLicense();
			if (!license) {
				auto key = licenseManager->getTrialLicense();
				license = licenseManager->activateLicense(key);
			}
			license->localCheck();

#pragma push_macro("check")
#undef check
            auto installFile = license->check(); // check license on the server
#pragma pop_macro("check")
		}
		catch( const std::exception& exc )
        {
            UE_LOG(LogTemp, Error, TEXT("Licensing failed!"));
        }

	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
	}
}

void FLicenseSpringPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(ExampleLibraryHandle);
	ExampleLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLicenseSpringPluginModule, LicenseSpringPlugin)
