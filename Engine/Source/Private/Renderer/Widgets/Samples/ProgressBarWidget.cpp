// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "Renderer/Widgets/Samples/ProgressBarWidget.h"

static const FVector2D<float> ProgressBarTargetRange = { 0, 1.f };

FProgressBarWidget::FProgressBarWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder)
	: FWidget(InWidgetManagementInterface, InWidgetName, InWidgetOrder)
	, CurrentProgressBarValue(0.5f)
	, ProgressBarRange(0.f, 1.f)
	, ProgressBarOrientation(EProgressBarOrientation::Horizontal)
	, ProgressBarColorFill(FColorRGBA::ColorLightGreen())
	, ProgressBarColorNotFill(FColorRGBA::ColorGray())
{
}

void FProgressBarWidget::Render()
{
	Super::Render();

	// Draw
	FRenderer* Renderer = GetRenderer();
	if (Renderer != nullptr)
	{
		// Set background color for whole progress bar
		Renderer->DrawRectangle(DefaultLocation, DefaultSize, ProgressBarColorNotFill);

		// Now draw percent of bar on top of whole bar
		Renderer->DrawRectangle(CurrentLocation, CurrentSize, ProgressBarColorFill);
	}
}

void FProgressBarWidget::RebuildWidget()
{
	Super::RebuildWidget();

	DefaultLocation = GetWidgetLocation();
	DefaultSize = GetWidgetSize();

	UpdateCurrentProgressBarSizeAndLocation();
}

void FProgressBarWidget::SetProgressBarPercent(const float NewProgressBarValue)
{
	CurrentProgressBarValue = NewProgressBarValue;

	UpdateCurrentProgressBarSizeAndLocation();
}

void FProgressBarWidget::SetProgressBarRange(const FVector2D<float>& NewProgressBarRange)
{
	ProgressBarRange = NewProgressBarRange;

	UpdateCurrentProgressBarSizeAndLocation();
}

void FProgressBarWidget::SetProgressBarOrientation(const EProgressBarOrientation InProgressBarOrientation)
{
	ProgressBarOrientation = InProgressBarOrientation;

	UpdateCurrentProgressBarSizeAndLocation();
}

void FProgressBarWidget::SetProgressBarColorFill(const FColorRGBA& NewColor)
{
	ProgressBarColorFill = NewColor;
}

void FProgressBarWidget::SetProgressBarColorNotFill(const FColorRGBA& NewColor)
{
	ProgressBarColorNotFill = NewColor;
}

void FProgressBarWidget::UpdateCurrentProgressBarSizeAndLocation()
{
	CurrentLocation = DefaultLocation;
	CurrentSize = DefaultSize;

	const float NormalizedValue = FMath::MapRange(CurrentProgressBarValue, ProgressBarRange, ProgressBarTargetRange);

	switch (ProgressBarOrientation)
	{
		case EProgressBarOrientation::Vertical:
		{
			CurrentSize.Y = FMath::RoundToInt(NormalizedValue * static_cast<float>(DefaultSize.Y));

			break;
		}
		case EProgressBarOrientation::Horizontal:
		{
			CurrentSize.X = FMath::RoundToInt(NormalizedValue * static_cast<float>(DefaultSize.X));

			break;
		}
	}
}
