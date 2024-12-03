#pragma once

#include "CoreMinimal.h"

/**
 * Widget that is used to create space between other widgets
 */
class FSpacerWidget : public FWidget
{
public:
	FSpacerWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, int32 InWidgetOrder);


};
