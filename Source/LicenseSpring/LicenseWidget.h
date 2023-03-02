// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LicenseSpringPlugin.h"
#include "Blueprint/UserWidget.h"
#include "LicenseWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LICENSESPRING_API ULicenseWidget : public UUserWidget
{
	GENERATED_BODY()

	TSharedPtr<ILicenseManager> LicenseManager;
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* InfoText;

	UPROPERTY(meta=(BindWidget))
	class UEditableText* ApiKeyText;

	UPROPERTY(meta=(BindWidget))
	class UEditableText* SharedKeyText;

	UPROPERTY(meta=(BindWidget))
	class UEditableText* ProductCodeText;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* LicenseKeyText;

	UPROPERTY(meta=(BindWidget))
	class UButton* LicenseButton;

public:
	UFUNCTION()
	void OnLicenseButtonClicked();
};
