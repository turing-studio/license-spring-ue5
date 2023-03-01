#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LICENSESPRING_API SULicenseGeneratorWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SULicenseGeneratorWidget)
	{
	}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
