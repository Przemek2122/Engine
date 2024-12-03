// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "ECS/ComponentManagerInterface.h"

IComponentManagerInterface::IComponentManagerInterface(IComponentManagerInterface* InComponentManagerInterfaceParent, FWindow* InOwnerWindow)
	: ComponentManagerInterfaceParent(InComponentManagerInterfaceParent)
	, bDoesHaveComponentManagerInterfaceParent(InComponentManagerInterfaceParent != nullptr)
	, OwnerWindow(InOwnerWindow)
	, bShouldCallBeginPlayOnNewComponents(false)
{
}

IComponentManagerInterface::~IComponentManagerInterface()
{
	Cleanup();
}

bool IComponentManagerInterface::DestroyComponent(const std::string& ComponentName)
{
	if (ComponentsMap.IsValidKey(ComponentName))
	{
		ComponentsMap[ComponentName]->EndPlay();

		OnComponentDestroy(ComponentName, ComponentsMap[ComponentName].get());

		ComponentsMap[ComponentName].reset();

		ComponentsMap.Remove(ComponentName);

		return true;
	}

	return false;
}

bool IComponentManagerInterface::DestroyComponentByInstance(const UBaseComponent* Component)
{
	for (std::pair<const std::string, std::shared_ptr<UBaseComponent>>& ComponentPair : ComponentsMap)
	{
		if (ComponentPair.second.get() == Component)
		{
			ComponentPair.second->EndPlay();

			OnComponentDestroy(ComponentPair.first, ComponentPair.second.get());

			ComponentPair.second.reset();

			ComponentsMap.Remove(ComponentPair.first);

			return true;
		}
	}

	return false;
}

void IComponentManagerInterface::OnComponentCreated(const std::string& ComponentName, UBaseComponent* NewComponent)
{
}

void IComponentManagerInterface::OnComponentDestroy(const std::string& ComponentName, UBaseComponent* OldComponent)
{
}

IComponentManagerInterface* IComponentManagerInterface::GetOwner() const
{
	return ComponentManagerInterfaceParent;
}

IComponentManagerInterface* IComponentManagerInterface::GetOwnerTop() const
{
	IComponentManagerInterface* CurrentChainElement = ComponentManagerInterfaceParent;

	if (CurrentChainElement->HasOwner())
	{
		CurrentChainElement = CurrentChainElement->GetOwnerTop();
	}

	return CurrentChainElement;
}

FWindow* IComponentManagerInterface::GetOwnerWindow() const
{
	return OwnerWindow;
}

void IComponentManagerInterface::TickComponents(const float DeltaTime)
{
	for (const auto& [ComponentName, Component] : ComponentsMap)
	{
		if (Component->IsComponentActive())
		{
			Component->Tick(DeltaTime);
		}
	}
}

void IComponentManagerInterface::RenderComponents()
{
	for (const auto& [ComponentName, Component] : ComponentsMap)
	{
		if (Component->IsComponentActive())
		{
			Component->Render();
		}
	}
}

void IComponentManagerInterface::Cleanup()
{
	for (auto& [ComponentName, ComponentPtr] : ComponentsMap)
	{
		ComponentPtr->EndPlay();

		OnComponentDestroy(ComponentName, ComponentPtr.get());
	}

	ComponentsMap.Clear();
}
