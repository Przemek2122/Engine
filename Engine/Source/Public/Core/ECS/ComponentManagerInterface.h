// Created by Przemys³aw Wiewióra 2020

#pragma once
#include "BaseComponent.h"
#include "BaseComponent.h"

class UBaseComponent;

class IComponentManagerInterface
{
protected:
	IComponentManagerInterface(IComponentManagerInterface* InComponentManagerInterfaceParent, FWindow* InOwnerWindow);
	virtual ~IComponentManagerInterface();

public:
	template<class TComponentClass, typename... TInParams>
	TComponentClass* CreateComponent(std::string ComponentName, TInParams... InParams)
	{
		std::shared_ptr<TComponentClass> NewComponent = std::make_shared<TComponentClass>(this, InParams ...);

		ComponentsMap.Emplace(ComponentName, NewComponent);

		if (bShouldCallBeginPlayOnNewComponents)
		{
			NewComponent.get()->BeginPlay();
		}

		OnComponentCreated(ComponentName, NewComponent.get());

		return NewComponent.get();
	}

	bool DestroyComponent(const std::string& ComponentName);
	bool DestroyComponentByInstance(const UBaseComponent* Component);

	/** Try to avoid getting by name, eg cache in property */
	template<typename TComponentClass = UBaseComponent>
	TComponentClass* GetComponent(std::string ComponentName)
	{
		if (ComponentsMap.IsValidKey(ComponentName))
		{
			return ComponentsMap[ComponentName];
		}

		return nullptr;
	}

	/** Try to avoid getting by name, eg cache in property */
	template<typename TComponentClass = UBaseComponent>
	TComponentClass* GetComponent(const std::string& ComponentName)
	{
		if (ComponentsMap.IsValidKey(ComponentName))
		{
			return ComponentsMap[ComponentName];
		}

		return nullptr;
	}

	/** Another way of getting component, still try to avoid calling it often. */
	template<typename TComponentClass>
	TComponentClass* GetComponentByClass()
	{
		for (std::pair<const std::string, std::shared_ptr<UBaseComponent>>& ComponentPair : ComponentsMap)
		{
			TComponentClass* ComponentCasted = dynamic_cast<TComponentClass*>(ComponentPair.second.get());

			if (ComponentCasted != nullptr)
			{
				return ComponentCasted;
			}
		}

		// Search subcomponents if not present on top
		for (std::pair<const std::string, std::shared_ptr<UBaseComponent>>& ComponentPair : ComponentsMap)
		{
			TComponentClass* Component = ComponentPair.second.get()->GetComponentByClass<TComponentClass>();

			if (Component != nullptr)
			{
				return Component;
			}
		}

		return nullptr;
	}

	/** Called when new component is created. */
	virtual void OnComponentCreated(const std::string& ComponentName, UBaseComponent* NewComponent);

	/** Called before destroying component. */
	virtual void OnComponentDestroy(const std::string& ComponentName, UBaseComponent* OldComponent);

	/** @returns true if has owner and GetOwner() is safe to call. */
	_NODISCARD bool HasOwner() const { return bDoesHaveComponentManagerInterfaceParent; }

	/** @return cached owner. */
	_NODISCARD IComponentManagerInterface* GetOwner() const;

	/** Iterate all elements to the top returning top level owner, most likely EEntity. */
	_NODISCARD IComponentManagerInterface* GetOwnerTop() const;

	_NODISCARD FWindow* GetOwnerWindow() const;

	void TickComponents(const float DeltaTime);
	void RenderComponents();

	void Cleanup();

protected:
	/** Parent pointer - might be null! @see bool bDoesHaveComponentManagerInterfaceParent */
	IComponentManagerInterface* ComponentManagerInterfaceParent;

	/** Components accessible by strings passed when creating components which are component names. */
	CUnorderedMap<std::string, std::shared_ptr<UBaseComponent>> ComponentsMap;

	/** Cached in constructor (ComponentManagerInterfaceParent != nullptr) */
	bool bDoesHaveComponentManagerInterfaceParent;

	/** Window owner */
	FWindow* OwnerWindow;

	bool bShouldCallBeginPlayOnNewComponents;

};