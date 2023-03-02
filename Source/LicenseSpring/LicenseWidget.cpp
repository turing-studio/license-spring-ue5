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
	LicenseButton->OnClicked.AddDynamic(this, &ULicenseWidget::OnLicenseButtonClicked);
}

void ULicenseWidget::OnLicenseButtonClicked()
{
	FAppConfig AppConfig("Unreal C++ Sample", "1.0");
	
	LicenseManager = AppConfig.CreateLicenseManager(
		ApiKeyText->GetText().ToString(),
		SharedKeyText->GetText().ToString(),
		ProductCodeText->GetText().ToString()
	);

	const FText LicenseKey = LicenseKeyText->GetText();
	if (!LicenseKey.IsEmpty())
		LicenseManager->ActivateLicense(LicenseKey.ToString());

	TArray< FStringFormatArg > args;
	args.Add( FStringFormatArg( LicenseManager->GetAppName() ) );
	args.Add( FStringFormatArg( LicenseManager->GetAppVersion() ) );
	args.Add( FStringFormatArg( LicenseManager->GetSdkVersion() ) );
	args.Add( FStringFormatArg( LicenseManager->GetLicenseSpringAPIVersion() ) );
	args.Add( FStringFormatArg( LicenseManager->GetOsVersion() ) );
	args.Add( FStringFormatArg( LicenseManager->GetLicenseDetails() ) );
	
	const FString text = FString::Format(
		TEXT("AppName: {0}\n"
			"AppVersion: {1}\n"
			"LicenseSpring SDK version: {2}\n"
			"LicenseSpring API version: {3}\n"
			"Determined OS version: {4}\n"
			"Details: {5}"
			),
			args);
	
	InfoText->SetText(FText::FromString(text));
}
