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

	FInteractionBaseWidget::Render();
}

void FButtonWidget::RebuildWidget()
{
	FInteractionBaseWidget::RebuildWidget();

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
	FInteractionBaseWidget::NativeHoverInsideTick();
}

void FButtonWidget::NativeHoverOutsideTick()
{
	FInteractionBaseWidget::NativeHoverOutsideTick();

	ButtonRenderColor = ButtonNormalColor;
}

void FButtonWidget::NativePressLeft()
{
	FInteractionBaseWidget::NativePressLeft();
	
	ButtonRenderColor = ButtonClickColor;
}

void FButtonWidget::NativeReleaseLeft()
{
	FInteractionBaseWidget::NativeReleaseLeft();
	
	ButtonRenderColor = ButtonNormalColor;
}

void FButtonWidget::NativePressRight()
{
	FInteractionBaseWidget::NativePressRight();

	ButtonRenderColor = ButtonClickColor;
}

void FButtonWidget::NativeReleaseRight()
{
	FInteractionBaseWidget::NativeReleaseRight();

	ButtonRenderColor = ButtonNormalColor;
}

void FButtonWidget::NativeMouseEnterWidget()
{
	FInteractionBaseWidget::NativeMouseEnterWidget();
	
	ButtonRenderColor = ButtonHoverColor;
}

void FButtonWidget::NativeMouseExitWidget()
{
	FInteractionBaseWidget::NativeMouseExitWidget();
	
	ButtonRenderColor = ButtonNormalColor;
}

void FButtonWidget::UseDefaultSize()
{
	// Default size of button
	SetWidgetSizePercent( { 1.0, 0.2f }, EWidgetSizeType::ParentPercentage );
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
