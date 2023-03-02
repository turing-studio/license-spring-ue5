// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HAL/Platform.h"
#include "Modules/ModuleManager.h"

class LICENSESPRINGPLUGIN_API ILicenseManager
{
public:
	virtual ~ILicenseManager() = default;
	
	virtual FString GetAppName() const = 0;
	virtual FString GetAppVersion() const = 0;
	virtual FString GetSdkVersion() const = 0;
	virtual uint32_t GetLicenseSpringAPIVersion() const = 0;
	virtual FString GetOsVersion() const = 0;
	virtual FString GetLicenseDetails() const = 0;
	virtual void ActivateLicense(const FString& Key) const = 0;
};

class LICENSESPRINGPLUGIN_API FAppConfig
{
public:
	const FString AppName;
	const FString AppVersion;
	
	FAppConfig(const FString& AppName, const FString& Version) : AppName(AppName), AppVersion(Version) { }
	
	TSharedPtr<ILicenseManager> CreateLicenseManager(
		const FString& ApiKey, const FString& SharedKey, const FString& ProductCode) const;
};


class LICENSESPRINGPLUGIN_API FLicenseSpring : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
	/** Handle to the test dll we will load */
	void*	ExampleLibraryHandle;
};
