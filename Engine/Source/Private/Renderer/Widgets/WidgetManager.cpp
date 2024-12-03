// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Renderer/Widgets/WidgetManager.h"
#include "Renderer/Widgets/Widget.h"

FWidgetManager::FWidgetManager(FWindow* InOwnerWindow)
	: OwnerWindow(InOwnerWindow)
{
}

FWidgetManager::~FWidgetManager()
{
	for (FWidget* Widget : ManagedWidgets)
	{
		Widget->DestroyWidgetImmediate();
	}
}

void FWidgetManager::DeInit()
{
	ClearChildren();
}

void FWidgetManager::ReceiveTick()
{
	for (FWidget* ManagedWidget : ManagedWidgets)
	{
		if (ManagedWidget->NeedsWidgetRebuild())
		{
			FWidgetGeometry WidgetGeometry;
			ManagedWidget->GenerateWidgetGeometry(WidgetGeometry);

			ManagedWidget->RebuildWidget();
		}
	}

	TickWidgets();
}

FVector2D<int> FWidgetManager::GetWidgetManagerOffset() const
{
	return 0;
}

FVector2D<int> FWidgetManager::GetWidgetManagerSize() const
{
	return OwnerWindow->GetWindowSize();
}

bool FWidgetManager::HasParent() const
{
	return false;
}

int32 FWidgetManager::GetParentsNumber() const
{
	return 0;
}

IWidgetManagementInterface* FWidgetManager::GetParent() const
{
	return nullptr;
}

FWindow* FWidgetManager::GetOwnerWindow() const
{
	return OwnerWindow;
}

void FWidgetManager::OnWindowChanged()
{
	for (FWidget* Widget : ManagedWidgets)
	{
		Widget->OnWindowChanged();
	}
}

void FWidgetManager::OnWindowSizeChanged()
{
	for (FWidget* Widget : ManagedWidgets)
	{
		Widget->RequestWidgetRebuild();
	}
}
