// Copyright Epic Games, Inc. All Rights Reserved.

#include "LicenseSpringPlugin.h"

#include <sstream>

#include "Configuration.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "LicenseManager.h"

#define LOCTEXT_NAMESPACE "FLicenseSpringPluginModule"

std::string TmToStr( const tm& dateTime )
{
	return LicenseSpring::TmToString( dateTime, "%d-%m-%Y %H:%M:%S" );
};

static FString PrintLicenseDetails(LicenseSpring::License::ptr_t license )
{
	std::stringstream s;
	if( license == nullptr )
		return "";

	s << std::endl << "------------- License info -------------" << std::endl;

	auto formatStr = []( std::string& str, const std::string& value )
	{
		if( value.empty() )
			return;
		if( !str.empty() )
			str.append( " " );
		str.append( value );
	};

	const auto& licenseOwner = license->owner();
	{
		std::string ownerInfo; // license owner info string
		formatStr( ownerInfo, licenseOwner.firstName() );
		formatStr( ownerInfo, licenseOwner.lastName() );
		formatStr( ownerInfo, licenseOwner.email() );
		formatStr( ownerInfo, licenseOwner.company() );
		if( !ownerInfo.empty() )
			s << "Customer information (licensed to): " << ownerInfo << std::endl;
	}

	auto licenseUser = license->licenseUser();
	if( licenseUser )
	{
		std::string userInfo;
		formatStr( userInfo, licenseUser->firstName() );
		formatStr( userInfo, licenseUser->lastName() );
		formatStr( userInfo, licenseUser->email() );
		if( !userInfo.empty() )
			s << "License user information: " << userInfo << std::endl;
	}

	if( !license->key().empty() )
		s << "Key = " << license->key() << std::endl;

	if( !license->user().empty() )
		s << "User = " << license->user() << std::endl;
    
	s << "Type = " << license->type().toFormattedString() << std::endl;
	s << "Status = " << license->status() << std::endl;
	s << "IsActive = " << license->isActive() << std::endl;
	s << "IsEnabled = " << license->isEnabled() << std::endl;
	s << "IsTrial = " << license->isTrial() << std::endl;
	s << "IsFloating = " << license->isFloating() << std::endl;
	if( license->isFloating() )
	{
		s << "Current floating slots count = " << license->floatingInUseCount() << std::endl;
		s << "Overall floating slots count = " << license->maxFloatingUsers() << std::endl;
		auto endDate = TmToStr( license->floatingEndDateTime() );
		if( license->isBorrowed() )
			s << "The license is borrowed until: " << endDate << std::endl;
		else
		{
			s << "Registration of this floating license expires at: " << endDate << std::endl;
			if( license->maxBorrowTime() > 0 )
				s << "License can be borrowed for " << license->maxBorrowTime()
						  << " hours max" << std::endl;
			else
				s << "License borrowing is not allowed" << std::endl;
		}
	}
	s << "IsOfflineActivated = " << license->isOfflineActivated() << std::endl;
	s << "Times activated = " << license->timesActivated() << std::endl;
	s << "Max activations = " << license->maxActivations() << std::endl;
	s << "Transfer count = " << license->transferCount() << std::endl;

	if( !license->startDate().empty() )
		s << "Start date = " << license->startDate() << std::endl;
	s << "Validity Period = " << TmToStr( license->validityPeriod() ) << std::endl;
	s << "Validity Period UTC = " << TmToStr( license->validityPeriodUtc() ) << std::endl;
	s << "Days remaining till license expires = " << license->daysRemaining() << std::endl;
	if( license->type() == LicenseTypeSubscription )
		s << "Grace period = " << license->gracePeriod() << std::endl;
	s << "Maintenance period = " << TmToStr( license->maintenancePeriod() ) << std::endl;
	s << "Maintenance period UTC = " << TmToStr( license->maintenancePeriodUtc() ) <<std::endl;
	s << "Maintenance days remaining = " << license->maintenanceDaysRemaining() << std::endl;
	s << "Last online check date = " << TmToStr( license->lastCheckDate() ) << std::endl;
	s << "Days passed since last online check = " << license->daysPassedSinceLastCheck() << std::endl;

	auto productFeatures = license->features();
	if( !productFeatures.empty() )
	{
		s << "Product features available for this license:" << std::endl;
		for( auto feature : productFeatures )
			s << feature.toString() << std::endl;
	}

	auto dataFields = license->customFields();
	if( !dataFields.empty() )
	{
		s << "Custom data fields available for this license:" << std::endl;
		for( const auto& field : dataFields )
			s << "Data field - Name: " << field.fieldName() << ", Value: " << field.fieldValue() << std::endl;
	}

	const auto& userData = license->userData();
	if( !userData.empty() )
	{
		s << "User data for this license: " << std::endl;
		for( const auto& field : userData )
			s << "Data field - Name: " << field.fieldName() << ", Value: " << field.fieldValue() << std::endl;
	}

	if( license->type() == LicenseTypeConsumption )
	{
		s << "Total consumptions = " << license->totalConsumption() << std::endl;
		s << "Max consumptions = " << license->maxConsumption() << std::endl;
		s << "Is overages allowed = " << license->isOveragesAllowed() << std::endl;
		if( license->isOveragesAllowed() )
			s << "Max overages = " << license->maxOverages() << std::endl;
	}

	return s.str().c_str();
}


class FLicenseManager : public ILicenseManager
{
	LicenseSpring::LicenseManager::ptr_t Manager;

	LicenseSpring::Configuration::ptr_t Configuration() const { return Manager->currentConfig(); }

public:
	FLicenseManager(LicenseSpring::LicenseManager::ptr_t Manager) : Manager(Manager) { }

	virtual FString GetAppName() const override { return Configuration()->getAppName().c_str(); }
	virtual FString GetAppVersion() const override { return Configuration()->getAppVersion().c_str(); }
	virtual FString GetSdkVersion() const override { return Configuration()->getSdkVersion().c_str(); }
	virtual uint32_t GetLicenseSpringAPIVersion() const override { return Configuration()->getLicenseSpringAPIVersion(); }
	virtual FString GetOsVersion() const override { return Configuration()->getOsVersion().c_str(); }
	virtual FString GetLicenseDetails() const override { return PrintLicenseDetails(Manager->getCurrentLicense()); }

	virtual void ActivateLicense(const FString& Key) const override
	{
		Manager->activateLicense(LicenseSpring::LicenseID::fromKey(TCHAR_TO_UTF8(*Key)));
	}
	
	static TSharedPtr<ILicenseManager> Create( const FString& ApiKey,
		const FString& SharedKey,
		const FString& ProductCode,
		const FString& AppName,
		const FString& AppVersion)
	{
		LicenseSpring::ExtendedOptions options;
		options.collectNetworkInfo( true );
		options.enableSSLCheck( true );

		const auto Conf = LicenseSpring::Configuration::Create(
			TCHAR_TO_UTF8(*ApiKey),
			TCHAR_TO_UTF8(*SharedKey),
			TCHAR_TO_UTF8(*ProductCode),
			TCHAR_TO_UTF8(*AppName),
			TCHAR_TO_UTF8(*AppVersion),
			options);
		
		return MakeShared<FLicenseManager>(LicenseSpring::LicenseManager::create(Conf));
	}
};

TSharedPtr<ILicenseManager> FAppConfig::CreateLicenseManager(
	const FString& ApiKey, const FString& SharedKey, const FString& ProductCode) const
{
	return FLicenseManager::Create(ApiKey, SharedKey, ProductCode, AppName, AppVersion);
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
		catch( LicenseSpring::LicenseSpringException exc )
		{
			UE_LOG(LogTemp, Error, TEXT("LicenseSpring exception %s"), exc.what());
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
