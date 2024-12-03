// Created by Przemysław Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

class FWidget;

/**
 * Class for managing window input.
 * (Only on focused window)
 */
class IWidgetInteraction
{
public:
	IWidgetInteraction();
	virtual ~IWidgetInteraction();
	
protected:
	void TickInteraction();
	virtual void GetWidgetsForInteraction(CArray<FWidget*>& Widgets) = 0;


protected:
	CArray<FWidget*> InteractiveWidgets;
	
};
