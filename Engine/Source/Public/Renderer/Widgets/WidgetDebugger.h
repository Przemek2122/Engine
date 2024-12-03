// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

#if WITH_WIDGET_DEBUGGER
/**
 * UI Debugger
 */
class FWidgetDebugger
{
public:
	FWidgetDebugger(FWindow* InWindow);
	virtual ~FWidgetDebugger() = default;

	void StartDebugger();

protected:
	void RefreshDisplayedWidgets();
	void CreateDebuggersForWidgets(FVerticalBoxWidget* InVerticalBox, const CArray<FWidget*>& InWidgets, int32 Depth);

	void CreateSingleDebuggerForWidget(FWidget* Widget);

protected:
	/** Debugged window */
	FWindow* Window;

	/** Window created for debugging primary Window */
	FWindow* DebuggerWindow;

	/** Currently debugged widget pointer set in CreateSingleDebuggerForWidget */
	FWidget* CurrentlyDebuggedWidget;
	
};
#endif