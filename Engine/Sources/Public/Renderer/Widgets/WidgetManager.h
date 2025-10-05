// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "WidgetsManagementInterface.h"

/**
 * Interactive UI rendering.
 */
class ENGINE_API FWidgetManager : public IWidgetManagementInterface
{
	friend FWindow;
	
protected:
	FWidgetManager(FWindow* InOwnerWindow);
	virtual ~FWidgetManager() override;

	virtual void DeInit();
	virtual void ReceiveTick();

	/** Begin IWidgetManagementInterface */
	NO_DISCARD FVector2D<int> GetWidgetManagerOffset() const override;
	NO_DISCARD FVector2D<int> GetWidgetManagerSize() const override;
	NO_DISCARD bool HasParent() const override;
	NO_DISCARD int32 GetParentsNumber() const override;
	NO_DISCARD IWidgetManagementInterface* GetParent() const override;
	NO_DISCARD FWindow* GetOwnerWindow() const override;
	void OnWindowChanged() override;
	/** End IWidgetManagementInterface */

	void OnWindowSizeChanged();

private:
	/** Window where widgets are rendered */
	FWindow* OwnerWindow;

	/** Window size */
	FVector2D<int> WidgetManagerSize;
	
};
