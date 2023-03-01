// Fill out your copyright notice in the Description page of Project Settings.


#include "LicenseWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/MultiLineEditableText.h"
#include "Components/TextBlock.h"
#include "LicenseSpringPlugin.h"


void ULicenseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LicenseButton->SetDisplayLabel("License test");
	InfoText->SetText(FText::FromString(TEXT("Test text license.")));
	LicenceInputText->SetText(FText::FromString(TEXT("Jako velki test")));
	// InfoText->SetText(FText::FromString(TEXT("Test text license.")));
	
	FAppConfig AppConfig("Unreal C++ Sample", "1.0");
	
	auto pConfiguration = AppConfig.CreateLicenseSpringConfig();
	
	// std::cout << "------------- General info -------------" << std::endl;
	// std::cout << pConfiguration->GetAppName() + ' ' << pConfiguration->GetAppVersion() << std::endl;
	// std::cout << "LicenseSpring SDK version: " << pConfiguration->GetSdkVersion() << std::endl;
	// std::cout << "LicenseSpring API version: " << pConfiguration->GetLicenseSpringAPIVersion() << std::endl;
	// std::cout << "Determined OS version:     " << pConfiguration->GetOsVersion() << std::endl;
	// std::cout << std::endl;

	TArray< FStringFormatArg > args;
	args.Add( FStringFormatArg( pConfiguration->GetAppName() ) );
	args.Add( FStringFormatArg( pConfiguration->GetAppVersion() ) );
	args.Add( FStringFormatArg( pConfiguration->GetSdkVersion() ) );
	args.Add( FStringFormatArg( pConfiguration->GetLicenseSpringAPIVersion() ) );
	args.Add( FStringFormatArg( pConfiguration->GetOsVersion() ) );
	
	const FString text = FString::Format(
		TEXT("AppName: {0}\n"
			"AppVersion: {1}\n"
			"LicenseSpring SDK version: {2}\n"
			"LicenseSpring API version: {3}\n"
			"Determined OS version: {4}"
			),
			args);
	
	InfoText->SetText(FText::FromString(text));

	// InfoText->SetText(FText::FromString(TEXT("Test text license 3.")));

	// std::cout << "------------- Network info -------------" << std::endl;
	// std::cout << "Host name:   " << pConfiguration->getNetworkInfo().hostName() << std::endl;
	// std::cout << "Local IP:    " << pConfiguration->getNetworkInfo().ip() << std::endl;
	// std::cout << "MAC address: " << pConfiguration->getNetworkInfo().mac() << std::endl;
	// std::cout << std::endl;

	
	// LicenseSpring::Configuration::ptr_t pconf;
>>>>>>> 5f20b88 (Simple library wrappers)
}
