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
	LicenceInputText->SetText(FText::FromString(TEXT("Enter your license")));
	
	FAppConfig AppConfig("Unreal C++ Sample", "1.0");
	
	auto Configuration = AppConfig.CreateLicenseSpringConfig();

	TArray< FStringFormatArg > args;
	args.Add( FStringFormatArg( Configuration->GetAppName() ) );
	args.Add( FStringFormatArg( Configuration->GetAppVersion() ) );
	args.Add( FStringFormatArg( Configuration->GetSdkVersion() ) );
	args.Add( FStringFormatArg( Configuration->GetLicenseSpringAPIVersion() ) );
	args.Add( FStringFormatArg( Configuration->GetOsVersion() ) );
	
	const FString text = FString::Format(
		TEXT("AppName: {0}\n"
			"AppVersion: {1}\n"
			"LicenseSpring SDK version: {2}\n"
			"LicenseSpring API version: {3}\n"
			"Determined OS version: {4}"
			),
			args);
	
	InfoText->SetText(FText::FromString(text));
}
