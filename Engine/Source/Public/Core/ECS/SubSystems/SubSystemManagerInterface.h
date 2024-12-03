// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"
#include "SubSystemInstanceInterface.h"

/**
 * Interface for SubSystem manager
 */
class ISubSystemManagerInterface
{
public:
	ISubSystemManagerInterface() = default;
	virtual ~ISubSystemManagerInterface();

	void TickSubSystems();
	void RenderSubSystems();

	/**
	 * Get SubSystem by given class.
	 * @tparam TSubSystemClass Class to search for.
	 * @return Object if found. nullptr otherwise.
	 */
	template<typename TSubSystemClass>
	TSubSystemClass* GetSubSystemByClass() const
	{
		static_assert(std::is_base_of_v<ISubSystemInstanceInterface, TSubSystemClass>, "SubSystem must inherit from ISubSystemInstanceInterface.");

		TSubSystemClass* Object = nullptr;
			
		for (ISubSystemInstanceInterface* SubSystemInstanceInterface : SubSystemsArray)
		{
			if (TSubSystemClass* CastedObject = dynamic_cast<TSubSystemClass*>(SubSystemInstanceInterface))
			{
				Object = CastedObject;
			}
		}

		return Object;
	}

	/** Create subsystem by given class with optional parameters */
	template<typename TSubSystemClass, typename... TInParams>
	TSubSystemClass* CreateSubSystem(TInParams... InParams)
	{
		static_assert(std::is_base_of_v<ISubSystemInstanceInterface, TSubSystemClass>, "SubSystem must inherit from ISubSystemInstanceInterface.");

		TSubSystemClass* NewEntity = new TSubSystemClass(InParams...);

		NewEntity->SetSubSystemManager(this);

		return NewEntity;
	}

	void RegisterSubSystem(ISubSystemInstanceInterface* InSubSystemInstanceInterface);
	void UnRegisterSubSystem(ISubSystemInstanceInterface* InSubSystemInstanceInterface);

protected:
	CArray<ISubSystemInstanceInterface*> SubSystemsArray;

};
