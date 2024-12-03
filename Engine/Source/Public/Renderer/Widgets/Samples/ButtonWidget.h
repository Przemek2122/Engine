// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "InteractionBaseWidget.h"

enum class EHorizontalAlignMethod : Uint8
{
	AlignToFit,
	AlignOnlyIfNotFitting
};

/**
 * Button widget
 * Contains background with customizable colors and clicking ability
 */
class FButtonWidget : public FInteractionBaseWidget
{
public:
	FButtonWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int InWidgetOrder = WIDGET_DEFINES_DEFAULT_ORDER);

	/** Begin FWidget */
	void Render() override;
	/** End FWidget */

	/** IWidgetPositionInterface FWidget */
	void RebuildWidget() override;
	/** End IWidgetPositionInterface */
	
	/** Begin FInteractionBaseWidget */
	void NativeHoverInsideTick() override;
	void NativeHoverOutsideTick() override;
	void NativePressLeft() override;
	void NativeReleaseLeft() override;
	void NativePressRight() override;
	void NativeReleaseRight() override;
	void NativeMouseEnterWidget() override;
	void NativeMouseExitWidget() override;
	/** End FInteractionBaseWidget */

	void UseDefaultSize();
	void SetScaleHorizontally(const bool bInScaleHorizontally);

	FColorRGBA GetButtonRenderColor() const { return ButtonRenderColor; }
	FColorRGBA GetButtonNormalColor() const { return ButtonNormalColor; }
	FColorRGBA GetButtonHoverColor() const { return ButtonHoverColor; }
	FColorRGBA GetButtonClickColor() const { return ButtonClickColor; }

	void SetButtonRenderColor(const FColorRGBA& Color);
	void SetButtonNormalColor(const FColorRGBA& Color);
	void SetButtonHoverColor(const FColorRGBA& Color);
	void SetButtonClickColor(const FColorRGBA& Color);

protected:
	FColorRGBA ButtonRenderColor;
	FColorRGBA ButtonNormalColor;
	FColorRGBA ButtonHoverColor;
	FColorRGBA ButtonClickColor;

	/** Should scale horizontally to fit children? */
	bool bScaleHorizontally;

	EHorizontalAlignMethod HorizontalAlignMethod;
	
};
