// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Renderer/Widgets/Samples/ButtonWidget.h"

FButtonWidget::FButtonWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int InWidgetOrder)
	: FInteractionBaseWidget(InWidgetManagementInterface, InWidgetName, InWidgetOrder)
	, ButtonRenderColor(FColorRGBA::ColorGray())
	, ButtonNormalColor(FColorRGBA::ColorGray())
	, ButtonHoverColor(FColorRGBA::ColorLightGray())
	, ButtonClickColor(FColorRGBA::ColorDarkGray())
	, bScaleHorizontally(true)
	, HorizontalAlignMethod(EHorizontalAlignMethod::AlignOnlyIfNotFitting)
{
}

void FButtonWidget::Render()
{
	GetRenderer()->DrawRectangle(GetWidgetLocation(EWidgetOrientation::Absolute), GetWidgetSize(), ButtonRenderColor, false);

	Super::Render();
}

void FButtonWidget::RebuildWidget()
{
	Super::RebuildWidget();

	if (HorizontalAlignMethod == EHorizontalAlignMethod::AlignToFit)
	{
		FVector2D<int32> NewSize = GetWidgetSize();
		NewSize.X = DesiredWidgetGeometry.Size.X;

		SetWidgetSize(NewSize, true);
	}
	else if (HorizontalAlignMethod == EHorizontalAlignMethod::AlignOnlyIfNotFitting)
	{
		FVector2D<int32> NewSize = GetWidgetSize();

		if (DesiredWidgetGeometry.Size.X > NewSize.X)
		{
			NewSize.X = DesiredWidgetGeometry.Size.X;

			SetWidgetSize(NewSize, true);
		}
	}
}

void FButtonWidget::NativeHoverInsideTick()
{
	Super::NativeHoverInsideTick();
}

void FButtonWidget::NativeHoverOutsideTick()
{
	Super::NativeHoverOutsideTick();

	ButtonRenderColor = ButtonNormalColor;
}

void FButtonWidget::NativePressLeft()
{
	Super::NativePressLeft();
	
	ButtonRenderColor = ButtonClickColor;
}

void FButtonWidget::NativeReleaseLeft()
{
	Super::NativeReleaseLeft();
	
	ButtonRenderColor = ButtonNormalColor;
}

void FButtonWidget::NativePressRight()
{
	Super::NativePressRight();

	ButtonRenderColor = ButtonClickColor;
}

void FButtonWidget::NativeReleaseRight()
{
	Super::NativeReleaseRight();

	ButtonRenderColor = ButtonNormalColor;
}

void FButtonWidget::NativeMouseEnterWidget()
{
	Super::NativeMouseEnterWidget();
	
	ButtonRenderColor = ButtonHoverColor;
}

void FButtonWidget::NativeMouseExitWidget()
{
	Super::NativeMouseExitWidget();
	
	ButtonRenderColor = ButtonNormalColor;
}

void FButtonWidget::UseDefaultSize()
{
	// Default size of button
	SetWidgetSize({ 180, 40 });
}

void FButtonWidget::SetScaleHorizontally(const bool bInScaleHorizontally)
{
	bScaleHorizontally = bInScaleHorizontally;
}

void FButtonWidget::SetButtonRenderColor(const FColorRGBA& Color)
{
	ButtonRenderColor = Color;
}

void FButtonWidget::SetButtonNormalColor(const FColorRGBA& Color)
{
	ButtonNormalColor = Color;
}

void FButtonWidget::SetButtonHoverColor(const FColorRGBA& Color)
{
	ButtonHoverColor = Color;
}

void FButtonWidget::SetButtonClickColor(const FColorRGBA& Color)
{
	ButtonClickColor = Color;
}
