// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "../Widget.h"

enum class EInputState;

enum class EHoverState : Uint8
{
	None,
	Entered,
	Exited
};

enum class EClickState : Uint8
{
	NotClicked,
	Pressed,
	Released
};

class FInteractionBaseWidget : public FWidget
{
public:
	FInteractionBaseWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder = WIDGET_DEFINES_DEFAULT_ORDER);

	/** Begin FWidget */
	void OnMouseMove(FVector2D<int> InMousePosition, EInputState InputState) override;
	bool OnMouseLeftClick(FVector2D<int> InMousePosition, EInputState InputState) override;
	bool OnMouseRightClick(FVector2D<int> InMousePosition, EInputState InputState) override;
	/** End FWidget */

	static _NODISCARD FVector2D<int> GetMouseLocation();

	/** Called once when pressed in widget area. */
	virtual void NativePressLeft();
	/** Called once when released. */
	virtual void NativeReleaseLeft();

	/** Called once when pressed in widget area. */
	virtual void NativePressRight();
	/** Called once when released. */
	virtual void NativeReleaseRight();

	/** Called every frame when hovered. */
	virtual void NativeHoverInsideTick();
	/** Called every frame when not hovered. */
	virtual void NativeHoverOutsideTick();

	/** Called when mouse enters this widget */
	virtual void NativeMouseEnterWidget();
	/** Called when mouse exits this widget */
	virtual void NativeMouseExitWidget();

	void CheckIfMouseIsInsideWidget();

	void SetHoverState(EHoverState NewHoverState);
	_NODISCARD EHoverState GetHoverState() const;
	EClickState GetClickStateLeftMouseButton() const;
	EClickState GetClickStateRightMouseButton() const;

	virtual void OnHoverStateChanged();

	virtual bool IsClickable() const;

	/** Called when mouse is over widget */
	FDelegate<> OnHoverStart;
	/** Called when mouse is not over widget anymore */
	FDelegate<> OnHoverEnd;

	/** Called when clicked with left mouse button - Press */
	FDelegate<> OnLeftClickPress;
	/** Called when clicked with left mouse button - Release */
	FDelegate<> OnLeftClickRelease;

	/** Called when clicked with right mouse button - Press */
	FDelegate<> OnRightClickPress;
	/** Called when clicked with right mouse button - Release */
	FDelegate<> OnRightClickRelease;

private:
	EHoverState HoverState;
	EClickState LeftClickState;
	EClickState RightClickState;

	/** Is mouse inside of this widget? (You can use @OnHover delegate instead) */
	bool bIsInWidget;

	/**  */
	bool bMouseEnteredWidget;
	
};
