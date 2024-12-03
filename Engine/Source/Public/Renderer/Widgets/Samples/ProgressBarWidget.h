// Created by Przemysław Wiewióra 2024

#pragma once

#include "CoreMinimal.h"
#include "../Widget.h"

/** Defines orientation of progress bar */
enum class EProgressBarOrientation
{
	Vertical,
	Horizontal
};

/**
 * Progress bar widget
 * used for displaying percent progress of something
 *
 * Note: You have to use SetSize to define size of ProgressBar. Default is 0 and will display nothing.
 */
class FProgressBarWidget : public FWidget
{
public:
	FProgressBarWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, int32 InWidgetOrder);

	/** Begin FWidget */
	void Render() override;
	void RebuildWidget() override;
	/** End FWidget */

	/** Set value of progress bar, value should be in range of ProgressBarRange (default is 0-1) */
	void SetProgressBarPercent(const float NewProgressBarValue);

	/** Set range of progress bar  (0-1 is default) */
	void SetProgressBarRange(const FVector2D<float>& NewProgressBarRange);

	/** Set progress bar to be vertical or horizontal (default) */
	void SetProgressBarOrientation(const EProgressBarOrientation InProgressBarOrientation);

	/** What is the color of progress bar where filled */
	void SetProgressBarColorFill(const FColorRGBA& NewColor);

	/** What is the color of progress bar where NOT filled */
	void SetProgressBarColorNotFill(const FColorRGBA& NewColor);

protected:
	void UpdateCurrentProgressBarSizeAndLocation();

	/** Current percent of progress bar */
	float CurrentProgressBarValue;

	/** What range is supported for CurrentProgressBarValue - Default would be 0.f, 1.f */
	FVector2D<float> ProgressBarRange;

	/** Is horizontal of vertical progressbar? */
	EProgressBarOrientation ProgressBarOrientation;

	/** What is the color of progress bar where filled */
	FColorRGBA ProgressBarColorFill;

	/** What is the color of progress bar where NOT filled */
	FColorRGBA ProgressBarColorNotFill;

	FVector2D<int32> DefaultLocation;
	FVector2D<int32> DefaultSize;

	FVector2D<int32> CurrentLocation;
	FVector2D<int32> CurrentSize;

};
