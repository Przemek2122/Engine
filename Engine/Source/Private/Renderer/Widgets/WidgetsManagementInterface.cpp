// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Renderer/Widgets/WidgetsManagementInterface.h"

IWidgetManagementInterface::IWidgetManagementInterface()
	: LastWidgetNumber(0)
	, bIsWidgetRequestingRebuild(false)
{
	OnAnyChildChangedDelegate.BindObject(this, &IWidgetManagementInterface::OnAnyChildChanged);
}

IWidgetManagementInterface::~IWidgetManagementInterface()
{
	ClearChildren();

	// Log any memory leak if encountered
	if (!ManagedWidgets.IsEmpty())
	{
		for (ContainerInt i = 0; i < ManagedWidgets.Size(); i++)
		{
			FWidget* CurrentWidget = ManagedWidgets[i];

			if (!CurrentWidget->IsPendingDelete())
			{
				LOG_ERROR("Memory leak detected, widget with name: '" << CurrentWidget->GetName() << "' has parent deleted but it's not destroyed.");
			}
		}
	}

	OnWidgetOrderChanged.UnBindAll();

	// This two arrays should always have the same size. Log if not.
	if (ManagedWidgets.Size() != ManagedWidgetsMap.Size())
	{
		LOG_ERROR("ManagedWidgets and ManagedWidgetsMap size mismatch.");
		LOG_ERROR("ManagedWidgets" << ManagedWidgets.Size());
		LOG_ERROR("ManagedWidgetsMap" << ManagedWidgetsMap.Size());
	}
}

void IWidgetManagementInterface::TickWidgets()
{
	ContainerInt Size = ManagedWidgets.Size();
	
	for (ContainerInt i = 0; i < Size; i++)
	{
		ManagedWidgets[i]->ReceiveTick();
	}
}

void IWidgetManagementInterface::RenderWidgets()
{
	const ContainerInt Size = ManagedWidgets.Size();
	
	for (ContainerInt i = 0; i < Size; i++)
	{
		FWidget* CurrentWidget = ManagedWidgets[i];
		CurrentWidget->ReceiveRender();
	}
}

void IWidgetManagementInterface::RequestWidgetRebuild()
{
	if (!bIsWidgetRequestingRebuild)
	{
		// Parent is required as otherwise we will not be able to ever rebuild
		IWidgetManagementInterface* Parent = GetParent();
		if (Parent != nullptr)
		{
			bIsWidgetRequestingRebuild = true;

			if (!Parent->NeedsWidgetRebuild())
			{
				Parent->RequestWidgetRebuild();
			}

			// Propagate to children
			for (FWidget* ManagedWidget : ManagedWidgets)
			{
				if (!ManagedWidget->NeedsWidgetRebuild())
				{
					ManagedWidget->RequestWidgetRebuild();
				}
			}
		}
	}
}

void IWidgetManagementInterface::MarkAsWidgetRebuild()
{
	bIsWidgetRequestingRebuild = false;
}

void IWidgetManagementInterface::GenerateWidgetGeometry(FWidgetGeometry& InWidgetGeometry)
{
}

void IWidgetManagementInterface::RebuildWidget()
{
	MarkAsWidgetRebuild();
}

bool IWidgetManagementInterface::AddChild(FWidget* InWidget)
{
	if (InWidget != nullptr)
	{
		IWidgetManagementInterface* ParentInterface = InWidget->GetParent();

		// @TODO Double calls RegisterWidget
		if (ParentInterface != nullptr)
		{
			ParentInterface->UnRegisterWidget(InWidget);

			RegisterWidget(InWidget);

			RegisterWidgetPostInit(InWidget);

			return true;
		}
		else
		{
			LOG_WARN("Double called AddChild");
		}
	}

	return false;
}

bool IWidgetManagementInterface::DestroyChildWidget(FWidget* Widget)
{
	if (ENSURE_VALID(Widget != nullptr))
	{
		if (Widget->GetParent() == this)
		{
			Widget->DestroyWidget();

			OnChildWidgetDestroyed(Widget);

			return (ManagedWidgets.Remove(Widget) && ManagedWidgetsMap.Remove(Widget->GetName()));
		}
		else
		{
			LOG_ERROR("Tried destroy child when calling on wrong parent");
		}
	}
	
	return false;
}

bool IWidgetManagementInterface::DestroyChildWidgetByName(const std::string& InWidgetName)
{
	if (ManagedWidgetsMap.ContainsKey(InWidgetName))
	{
		FWidget* Widget = GetWidgetByName(InWidgetName);
		if (Widget != nullptr)
		{
			return DestroyChildWidget(Widget);
		}
	}

	return false;
}

void IWidgetManagementInterface::ClearChildren()
{
	if (!ManagedWidgets.IsEmpty())
	{
		for (ContainerInt i = ManagedWidgets.Size() - 1; i >= 0; i--)
		{
			ManagedWidgets[i]->DestroyWidget();
		}
	}
}

ContainerInt IWidgetManagementInterface::GetChildrenCount() const
{
	return ManagedWidgets.Size();
}

FWidget* IWidgetManagementInterface::GetWidgetByName(const std::string& InWidgetName)
{
	return ManagedWidgetsMap.FindValueByKey(InWidgetName);
}

bool IWidgetManagementInterface::HasWidget(const std::string& InWidgetName)
{
	return ManagedWidgetsMap.ContainsKey(InWidgetName);
}

bool IWidgetManagementInterface::HasWidget(FWidget* InWidget)
{
	return ManagedWidgetsMap.ContainsValue(InWidget);
}

void IWidgetManagementInterface::OnChildWidgetCreated(FWidget* NewWidget)
{
	NewWidget->Init();

	AddChild(NewWidget);

	OnAnyChildChangedDelegate.Execute();

	if (HasParent())
	{
		IWidgetManagementInterface* Parent = GetParent();
		Parent->OnAnyChildChangedDelegate.Execute();
	}
}

void IWidgetManagementInterface::OnChildWidgetDestroyed(FWidget* NewWidget)
{
	OnAnyChildChangedDelegate.Execute();

	if (HasParent())
	{
		IWidgetManagementInterface* Parent = GetParent();
		Parent->OnAnyChildChangedDelegate.Execute();
	}
}

void IWidgetManagementInterface::OnChildSizeChanged()
{
}

void IWidgetManagementInterface::OnAnyChildChanged()
{
	if (HasParent())
	{
		IWidgetManagementInterface* Parent = GetParent();
		Parent->OnAnyChildChangedDelegate.Execute();
	}
}

void IWidgetManagementInterface::ChangeWidgetOrder(FWidget* InWidget)
{
	const ContainerInt ManagedWidgetsNum = ManagedWidgets.Size();
	const int WidgetOrder = InWidget->GetWidgetOrder();

	for (ContainerInt i = 0; i < ManagedWidgetsNum; i++)
	{
		FWidget* CurrentWidget = ManagedWidgets[i];
		
		if (CurrentWidget->GetWidgetOrder() < WidgetOrder)
		{
			ManagedWidgets.Remove(InWidget);
			ManagedWidgets.InsertAt(i, CurrentWidget);

			OnWidgetOrderChanged.Execute(InWidget);
		}
	}	
}

void IWidgetManagementInterface::RegisterWidget(FWidget* Widget)
{
#if _DEBUG
	const auto WidgetIndex = ManagedWidgets.FindIndexOf(Widget);
	ENSURE_VALID(WidgetIndex == -1);
#endif
	
	ManagedWidgets.InsertByLambda(Widget, [&](FWidget* ArrayObject, FWidget* Localobject)
	{
		if (ArrayObject->GetWidgetOrder() > Localobject->GetWidgetOrder())
		{
			return true;
		}

		return false;
	});
	ManagedWidgetsMap.InsertOrAssign(Widget->GetName(), Widget);

	if (Widget->GetParent() != this)
	{
		// This means re-register to same parent and it's not supported
		ENSURE_VALID(false);
	}

	RequestWidgetRebuild();
}

void IWidgetManagementInterface::RegisterWidgetPostInit(FWidget* Widget)
{
}

void IWidgetManagementInterface::UnRegisterWidget(FWidget* Widget)
{
#if _DEBUG
	const auto IndexToDelete = ManagedWidgets.FindIndexOf(Widget);
#endif
	
	ManagedWidgets.Remove(Widget);
	ManagedWidgetsMap.Remove(Widget->GetName());

	RequestWidgetRebuild();

#if _DEBUG
	const auto NotDeletedIndex = ManagedWidgets.FindIndexOf(Widget);
	ENSURE_VALID(NotDeletedIndex == -1);
#endif
}
