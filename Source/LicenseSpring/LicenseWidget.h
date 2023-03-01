// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LicenseWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LICENSESPRING_API ULicenseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
		class UTextBlock* InfoText;

	UPROPERTY(meta=(BindWidget))
		class UButton* LicenseButton;
};
