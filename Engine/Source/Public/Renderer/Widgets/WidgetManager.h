// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "WidgetsManagementInterface.h"

/**
 * Interactive UI rendering.
 */
class FWidgetManager : public IWidgetManagementInterface
{
	friend FWindow;
	
protected:
	FWidgetManager(FWindow* InOwnerWindow);
	virtual ~FWidgetManager() override;

	virtual void DeInit();
	virtual void ReceiveTick();

	/** Begin IWidgetManagementInterface */
	_NODISCARD FVector2D<int> GetWidgetManagerOffset() const override;
	_NODISCARD FVector2D<int> GetWidgetManagerSize() const override;
	_NODISCARD bool HasParent() const override;
	_NODISCARD int32 GetParentsNumber() const override;
	_NODISCARD IWidgetManagementInterface* GetParent() const override;
	_NODISCARD FWindow* GetOwnerWindow() const override;
	void OnWindowChanged() override;
	/** End IWidgetManagementInterface */

	void OnWindowSizeChanged();

private:
	/** Window where widgets are rendered */
	FWindow* OwnerWindow;

	/** Window size */
	FVector2D<int> WidgetManagerSize;
	
};
