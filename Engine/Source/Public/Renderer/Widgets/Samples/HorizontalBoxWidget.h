// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "../Widget.h"

enum class EHorizontalBoxAlignMethod
{
	/** One by one starting from the left of the right */
	FromTheLeft,
};

class FHorizontalBoxWidget : public FWidget
{
public:
	FHorizontalBoxWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder = WIDGET_DEFINES_DEFAULT_ORDER);

	/** Begin IWidgetManagementInterface interface */
	void GenerateDesiredWidgetGeometry() override;
	/** End IWidgetManagementInterface interface */

	void AlignWidgets();

	void SetScaleToContent(const bool bInScaleToContent);
	bool ShouldScaleToContent() const { return bScaleToContent; }

protected:
	void RebuildWidget() override;

	void AlignFromTheLeft();

	EHorizontalBoxAlignMethod HorizontalBoxAlignMethod;

	bool bScaleToContent;
	
};
