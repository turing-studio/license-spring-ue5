// Copyright Epic Games, Inc. All Rights Reserved.

#include "LicenseSpringPlugin.h"
#include "Configuration.h"
#include "Core.h"
#include "EncryptString.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "LicenseManager.h"

#define LOCTEXT_NAMESPACE "FLicenseSpringPluginModule"

class FConfiguration : public IConfiguration
{
	LicenseSpring::Configuration::ptr_t Configuration;

public:
	FConfiguration(LicenseSpring::Configuration::ptr_t conf) : Configuration(conf) { }

	virtual FString GetAppName() const override { return Configuration->getAppName().c_str(); }
	virtual FString GetAppVersion() const override { return Configuration->getAppVersion().c_str(); }
	virtual FString GetSdkVersion() const override { return Configuration->getSdkVersion().c_str(); }
	virtual uint32_t GetLicenseSpringAPIVersion() const override { return Configuration->getLicenseSpringAPIVersion(); }
	virtual FString GetOsVersion() const override { return Configuration->getOsVersion().c_str(); }

	static TSharedPtr<IConfiguration> Create( const std::string& apiKey,
		const std::string& sharedKey,
		const std::string& productCode,
		const std::string& appName,
		const std::string& appVersion)
	{
		LicenseSpring::ExtendedOptions options;
		options.collectNetworkInfo( true );
		options.enableSSLCheck( true );
		
		return MakeShared<FConfiguration>(LicenseSpring::Configuration::Create(apiKey, sharedKey, productCode, appName, appVersion, options));
	}
};

TSharedPtr<IConfiguration> FAppConfig::CreateLicenseSpringConfig() const
{
	return FConfiguration::Create(
		EncryptStr( "afce72fb-9fba-406e-8d19-ffde5b0a7cad" ), // your LicenseSpring API key (UUID)
		EncryptStr( "Qc8EdU7DY-gMI87-JMueZWXdtJ0Ek_hS6dGC_SwusO8" ), // your LicenseSpring Shared key
		EncryptStr( "DP" ), // product code that you specified in LicenseSpring for your application
	TCHAR_TO_UTF8(*AppName), TCHAR_TO_UTF8(*AppVersion) );
}

void FLicenseSpring::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("LicenseSpringPlugin")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/LicenseSpringPlugin/ThirdParty/LicenseSpringPluginLibrary/x64/Release/LicenseSpring.dll"));
#elif PLATFORM_MAC
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/LicenseSpringPlugin/ThirdParty/LicenseSpringPluginLibrary/Mac/Release/libLicenseSpring.dylib"));
#endif // PLATFORM_WINDOWS

	ExampleLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (ExampleLibraryHandle)
	{

		UE_LOG(LogTemp, Log, TEXT("Library handle aquired"));
		// Call the test function in the third party library that opens a message box
        LicenseSpring::Configuration::ptr_t pconf;
        try
        {
            // Values are copied from sample project
            pconf = LicenseSpring::Configuration::Create( "afce72fb-9fba-406e-8d19-ffde5b0a7cad",
                "Qc8EdU7DY-gMI87-JMueZWXdtJ0Ek_hS6dGC_SwusO8", "DP", "Cpp Sample", "1.1" );
        }
        catch( const std::exception& exc )
        {
			UE_LOG(LogTemp, Error, TEXT("%s"), exc.what());
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
			UE_LOG(LogTemp, Error, TEXT("Licensing failed! %s"), exc.what());
        }

	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
	}
}

void FLicenseSpring::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(ExampleLibraryHandle);
	ExampleLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLicenseSpring, LicenseSpringPlugin)
