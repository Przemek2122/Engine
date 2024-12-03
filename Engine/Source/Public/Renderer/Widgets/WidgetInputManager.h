#pragma once

#include "CoreMinimal.h"

#define WIDGET_INPUT_DEBUG_ORDER_ENABLED 1

enum class EInputState;

/**
 * Class responsible for giving input to the FWidget class
 */
class FWidgetInputManager
{
public:
	FWidgetInputManager(FWindow* InOwnerWindow);
	~FWidgetInputManager();

	/** Should be called when widget wants to receive input */
	void Register(FWidget* NewWidget, FDelegate<void, FWidgetInputManager*>& SetupDelegate);
	/** If it was registered it must be later unregistered */
	void UnRegister(FWidget* NewWidget, FDelegate<void, FWidgetInputManager*>& ClearDelegate);

protected:
	/** Function triggered when widget changes order to make sure it's always handle input in correct way, first visible, first takes input */
	void AddWidgetDependingOnOrder(FWidget* InputWidget);

	void SetupMouseDelegates();
	void ClearMouseDelegates();

	bool OnMouseMove(FVector2D<int> InLocation, EInputState InInputState);
	bool OnMouseLeftClick(FVector2D<int> InLocation, EInputState InInputState);
	bool OnMouseRightClick(FVector2D<int> InLocation, EInputState InInputState);

protected:
	/** Widgets which should be asked about input */
	CArray<FWidget*> WidgetsArray;

	FWindow* OwnerWindow;

};
