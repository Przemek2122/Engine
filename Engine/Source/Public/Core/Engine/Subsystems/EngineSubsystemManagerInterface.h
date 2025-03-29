// Created by Przemys³aw Wiewióra 2025

#pragma once
#include <memory>

#include "CoreMinimal.h"

class IEngineSubsystemInterface;

/** Class for managing engine subsystems */
class IEngineSubsystemManagerInterface
{
public:
	virtual ~IEngineSubsystemManagerInterface();

	void TickSubsystems();

	/** Create new Engine subsystem from given class */
	template<class TEngineSubsystemInterfaceClass>
	std::shared_ptr<TEngineSubsystemInterfaceClass> CreateEngineSubsystem()
	{
		std::shared_ptr<TEngineSubsystemInterfaceClass> Out = std::make_shared<TEngineSubsystemInterfaceClass>();

		Out->Initialize();

		ManagedEngineSubsystems.Push(Out);

		return Out;
	}

	/** Destroy Engine subsystem from by pointer */
	void DestroyEngineSubsystem(const std::shared_ptr<IEngineSubsystemInterface>& InEngineSubsystem);

	/** Get engine subsystem by class */
	template<class TEngineSubsystemInterfaceClass>
	std::shared_ptr<TEngineSubsystemInterfaceClass> GetSubsystemByClass()
	{
		std::shared_ptr<TEngineSubsystemInterfaceClass> Out;

		for (const std::shared_ptr<IEngineSubsystemInterface>& EngineSubsystemPtr : ManagedEngineSubsystems)
		{
			if (dynamic_cast<TEngineSubsystemInterfaceClass*>(EngineSubsystemPtr.get()) != nullptr)
			{
				Out = std::dynamic_pointer_cast<TEngineSubsystemInterfaceClass>(EngineSubsystemPtr);
			}
		}

		return Out;
	}

private:
	/** Array with managed subsystems */
	CArray<std::shared_ptr<IEngineSubsystemInterface>> ManagedEngineSubsystems;
	
};
