#include "CoreEngine.h"
#include "Renderer/Widgets/WidgetInputManager.h"

#include "Input/WindowInputManager.h"

FWidgetInputManager::FWidgetInputManager(FWindow* InOwnerWindow)
	: OwnerWindow(InOwnerWindow)
{
	if (InOwnerWindow != nullptr)
	{
		SetupMouseDelegates();
	}
}

FWidgetInputManager::~FWidgetInputManager()
{
	if (OwnerWindow != nullptr)
	{
		ClearMouseDelegates();
	}
}

void FWidgetInputManager::Register(FWidget* NewWidget, FDelegate<void, FWidgetInputManager*>& SetupDelegate)
{
	AddWidgetDependingOnOrder(NewWidget);

	SetupDelegate.Execute(this);

	if (IWidgetManagementInterface* Parent = NewWidget->GetParent())
	{
		Parent->OnWidgetOrderChanged.BindObject(this, &FWidgetInputManager::AddWidgetDependingOnOrder);
	}
}

void FWidgetInputManager::UnRegister(FWidget* NewWidget, FDelegate<void, FWidgetInputManager*>& ClearDelegate)
{
	if (IWidgetManagementInterface* Parent = NewWidget->GetParent())
	{
		Parent->OnWidgetOrderChanged.UnBindObject(this, &FWidgetInputManager::AddWidgetDependingOnOrder);
	}

	ClearDelegate.Execute(this);

	WidgetsArray.RemoveAll(NewWidget);
}

void FWidgetInputManager::AddWidgetDependingOnOrder(FWidget* InputWidget)
{
	// Make sure it does not exist already in array
	WidgetsArray.Remove(InputWidget);

	if (InputWidget->GetWidgetOrder() != WIDGET_DEFINES_DEFAULT_ORDER)
	{
		const ContainerInt ManagedWidgetsNum = WidgetsArray.Size();
		const int32 InputWidgetOrder = InputWidget->GetWidgetOrder();
		bool bWasInputWidgetAdded = false;

		for (ContainerInt i = 0; i < ManagedWidgetsNum; i++)
		{
			FWidget* CurrentWidget = WidgetsArray[i];

			if (InputWidgetOrder > CurrentWidget->GetWidgetOrder())
			{
				// Then add at current index
				WidgetsArray.InsertAt(i, InputWidget);

				bWasInputWidgetAdded = true;

				break;
			}
		}

		if (!bWasInputWidgetAdded)
		{
			WidgetsArray.Push(InputWidget);
		}
	}
	else
	{
		WidgetsArray.Push(InputWidget);
	}

#if WIDGET_INPUT_DEBUG_ORDER_ENABLED
	LOG_DEBUG("--- AddWidgetDependingOnOrder: " << OwnerWindow->GetWindowName() << " ---");

	for (FWidget* CurrentWidget : WidgetsArray)
	{
		if (!CurrentWidget->IsPendingDelete())
		{
			LOG_DEBUG("Widget" << CurrentWidget->GetName() << " has order: " << CurrentWidget->GetWidgetOrder());
		}
	}
#endif
}

void FWidgetInputManager::SetupMouseDelegates()
{
	FWindowInputManager* WindowInputManager = OwnerWindow->GetWindowInputManager();

	WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_MOVE")->Delegate.BindObject(this, &FWidgetInputManager::OnMouseMove);
	WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_BUTTON_LEFT")->Delegate.BindObject(this, &FWidgetInputManager::OnMouseLeftClick);
	WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_BUTTON_RIGHT")->Delegate.BindObject(this, &FWidgetInputManager::OnMouseRightClick);
}

void FWidgetInputManager::ClearMouseDelegates()
{
	FWindowInputManager* WindowInputManager = OwnerWindow->GetWindowInputManager();

	WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_MOVE")->Delegate.UnBindObject(this, &FWidgetInputManager::OnMouseMove);
	WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_BUTTON_LEFT")->Delegate.UnBindObject(this, &FWidgetInputManager::OnMouseLeftClick);
	WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_BUTTON_RIGHT")->Delegate.UnBindObject(this, &FWidgetInputManager::OnMouseRightClick);
}

bool FWidgetInputManager::OnMouseMove(FVector2D<int> InLocation, EInputState InInputState)
{
	FWidgetManager* WidgetManager = OwnerWindow->GetWidgetManager();
	CArray<FWidget*> ManagedWidgets = WidgetManager->GetManagedWidgets();
	for (FWidget* ManagedWidget : ManagedWidgets)
	{
		ManagedWidget->ReceiveOnMouseMove(InLocation, InInputState);
	}

	return false;
}

bool FWidgetInputManager::OnMouseLeftClick(FVector2D<int> InLocation, EInputState InInputState)
{
	bool bIsInputConsumed = false;

	FWidgetManager* WidgetManager = OwnerWindow->GetWidgetManager();
	CArray<FWidget*> ManagedWidgets = WidgetManager->GetManagedWidgets();
	for (FWidget* ManagedWidget : ManagedWidgets)
	{
		if (ManagedWidget->ReceiveOnMouseLeftClick(InLocation, InInputState))
		{
			bIsInputConsumed = true;
		}
	}

	return bIsInputConsumed;
}

bool FWidgetInputManager::OnMouseRightClick(FVector2D<int> InLocation, EInputState InInputState)
{
	bool bIsInputConsumed = false;

	FWidgetManager* WidgetManager = OwnerWindow->GetWidgetManager();
	CArray<FWidget*> ManagedWidgets = WidgetManager->GetManagedWidgets();
	for (FWidget* ManagedWidget : ManagedWidgets)
	{
		if (ManagedWidget->ReceiveOnMouseRightClick(InLocation, InInputState))
		{
			bIsInputConsumed = true;
		}
	}

	return bIsInputConsumed;
}
