// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Renderer/Widgets/Samples/InteractionBaseWidget.h"
#include "Input/EventHandler.h"

FInteractionBaseWidget::FInteractionBaseWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder)
	: FWidget(InWidgetManagementInterface, InWidgetName, InWidgetOrder)
	, HoverState(EHoverState::None)
	, LeftClickState(EClickState::NotClicked)
	, RightClickState(EClickState::NotClicked)
	, bIsInWidget(false)
	, bMouseEnteredWidget(false)
{
	SetWidgetInteraction(EWidgetInteraction::Interactive);
}
void FInteractionBaseWidget::OnMouseMove(FVector2D<int> InMousePosition, EInputState InputState)
{
	CheckIfMouseIsInsideWidget();
}

bool FInteractionBaseWidget::OnMouseLeftClick(FVector2D<int> InMousePosition, EInputState InputState)
{
	bool bIsInputConsumed = false;

	if (IsClickable())
	{
		if (LeftClickState == EClickState::NotClicked)
		{
			LeftClickState = EClickState::Pressed;

			NativePressLeft();

			bIsInputConsumed = true;
		}
		else if (LeftClickState == EClickState::Pressed)
		{
			LeftClickState = EClickState::Released;

			NativeReleaseLeft();

			bIsInputConsumed = true;
		}
	}

	return bIsInputConsumed;
}

bool FInteractionBaseWidget::OnMouseRightClick(FVector2D<int> InMousePosition, EInputState InputState)
{
	bool bIsInputConsumed = false;

	if (IsClickable())
	{
		if (RightClickState == EClickState::NotClicked)
		{
			RightClickState = EClickState::Pressed;

			NativePressRight();

			bIsInputConsumed = true;
		}
		else if (RightClickState == EClickState::Pressed)
		{
			RightClickState = EClickState::Released;

			NativeReleaseRight();

			bIsInputConsumed = true;
		}
	}

	return bIsInputConsumed;
}

FVector2D<int> FInteractionBaseWidget::GetMouseLocation()
{
	return GetEventHandler()->GetMouseLocationCurrent();
}

void FInteractionBaseWidget::NativePressLeft()
{
	OnLeftClickPress.Execute();
}

void FInteractionBaseWidget::NativeReleaseLeft()
{
	OnLeftClickRelease.Execute();

	CheckIfMouseIsInsideWidget();
}

void FInteractionBaseWidget::NativePressRight()
{
	OnRightClickPress.Execute();
}

void FInteractionBaseWidget::NativeReleaseRight()
{
	OnRightClickRelease.Execute();

	CheckIfMouseIsInsideWidget();
}
void FInteractionBaseWidget::NativeHoverInsideTick()
{
	if (LeftClickState == EClickState::Released)
	{
		LeftClickState = EClickState::NotClicked;
	}

	if (RightClickState == EClickState::Released)
	{
		RightClickState = EClickState::NotClicked;
	}
}

void FInteractionBaseWidget::NativeHoverOutsideTick()
{
}

void FInteractionBaseWidget::NativeMouseEnterWidget()
{
	OnHoverStart.Execute();
}

void FInteractionBaseWidget::NativeMouseExitWidget()
{
	if (LeftClickState == EClickState::Pressed)
	{
		LeftClickState = EClickState::NotClicked;
	}

	if (RightClickState == EClickState::Released)
	{
		RightClickState = EClickState::NotClicked;
	}

	OnHoverEnd.Execute();
}

void FInteractionBaseWidget::CheckIfMouseIsInsideWidget()
{
	const FVector2D<int> LastMouseLocation = GetMouseLocation();
	bIsInWidget = IsLocationInsideWidget(LastMouseLocation);

	if (bIsInWidget)
	{
		if (!bMouseEnteredWidget)
		{
			NativeMouseEnterWidget();

			bMouseEnteredWidget = true;
		}

		NativeHoverInsideTick();
	}
	else
	{
		if (bMouseEnteredWidget)
		{
			NativeMouseExitWidget();

			bMouseEnteredWidget = false;
		}

		NativeHoverOutsideTick();
	}
}

void FInteractionBaseWidget::SetHoverState(EHoverState NewHoverState)
{
	if (HoverState != NewHoverState)
	{
		HoverState = NewHoverState;
		
		OnHoverStateChanged();
	}
}

EHoverState FInteractionBaseWidget::GetHoverState() const
{
	return HoverState;
}

EClickState FInteractionBaseWidget::GetClickStateLeftMouseButton() const
{
	return LeftClickState;
}

EClickState FInteractionBaseWidget::GetClickStateRightMouseButton() const
{
	return RightClickState;
}

void FInteractionBaseWidget::OnHoverStateChanged()
{
	switch (HoverState)
	{
		case EHoverState::None:
		{
			ENSURE_VALID(false);

			break;
		}
		case EHoverState::Entered:
		{
			NativeMouseEnterWidget();

			break;
		}
		case EHoverState::Exited:
		{
			NativeMouseExitWidget();

			break;
		}
	}
}

bool FInteractionBaseWidget::IsClickable() const
{
	return (bIsInWidget && IsInteractive());
}
