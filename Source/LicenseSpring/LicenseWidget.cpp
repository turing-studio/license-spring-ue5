// Fill out your copyright notice in the Description page of Project Settings.


#include "LicenseWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"


void ULicenseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LicenseButton->SetDisplayLabel("License test");
	InfoText->SetText(FText::FromString(TEXT("Test text license.")));
}
