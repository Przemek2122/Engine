// Created by Przemys³aw Wiewióra 2020

#pragma once

enum class EBorderDisplayMethod
{
	/** Only draws a line starting at widget location and being draw as widget size */
	BorderOutline,

	/** Draws rectangle */
	BorderWithFill,
};

class FBorderWidget : public FWidget
{
public:
	FBorderWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder = WIDGET_DEFINES_DEFAULT_ORDER);

	/** Begin FWidget */
	void Render() override;
	/** End FWidget */

	void SetBorderDisplayMethod(EBorderDisplayMethod NewBorderDisplayMethod);
	void SetColor(const FColorRGBA& NewColor);

	EBorderDisplayMethod GetBorderDisplayMethod() const;
	FColorRGBA GetColor() const;

protected:
	/** Current border display method, see enum for all options */
	EBorderDisplayMethod CurrentBorderDisplayMethod;

	/** Current color for this widget, will be color of border or fill depending on settings */
	FColorRGBA CurrentWidgetColor;

};
