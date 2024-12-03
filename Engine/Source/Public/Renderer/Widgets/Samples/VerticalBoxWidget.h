// Created by Przemysław Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "../Widget.h"

enum class EVerticalBoxAlignMethod : uint8
{
	AlignToLeft,
	AlignToCenter,
};

class FVerticalBoxWidget : public FWidget
{
public:
	FVerticalBoxWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder = WIDGET_DEFINES_DEFAULT_ORDER);

	/** Begin IWidgetManagementInterface interface */
	void RebuildWidget() override;
	/** End IWidgetManagementInterface interface */

	void SetScaleToContent(const bool bNewScaleToContent);
	bool ShouldScaleToContent() const { return bScaleToContent; }
	
	void AlignWidgets();

	void SetVerticalBoxAlignMethod(const EVerticalBoxAlignMethod InVerticalBoxAlignMethod, const bool bUpdateAfterSet = true);
	_NODISCARD EVerticalBoxAlignMethod GetVerticalBoxAlignMethod() const;
		 
protected:
	void AlignToLeft();
	void AlignToCenter();

	EVerticalBoxAlignMethod VerticalBoxAlignMethod;

	bool bScaleToContent;

	ContainerInt CurrentNumberOfCalculatedChildren;
	
};
