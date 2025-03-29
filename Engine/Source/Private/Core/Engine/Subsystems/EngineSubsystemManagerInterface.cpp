// Created by Przemys³aw Wiewióra 2025

#include "CoreEngine.h"
#include "Engine/Subsystems/EngineSubsystemManagerInterface.h"
#include "Engine/Subsystems/EngineSubsystemInterface.h"

IEngineSubsystemManagerInterface::~IEngineSubsystemManagerInterface()
{
	for (const std::shared_ptr<IEngineSubsystemInterface>& EngineSubsystemPtr : ManagedEngineSubsystems)
	{
		EngineSubsystemPtr->DeInitialize();
	}
}

void IEngineSubsystemManagerInterface::TickSubsystems()
{
	for (const std::shared_ptr<IEngineSubsystemInterface>& EngineSubsystemPtr : ManagedEngineSubsystems)
	{
		EngineSubsystemPtr->Tick();
	}
}

void IEngineSubsystemManagerInterface::DestroyEngineSubsystem(const std::shared_ptr<IEngineSubsystemInterface>& InEngineSubsystem)
{
	InEngineSubsystem->DeInitialize();

	ManagedEngineSubsystems.Remove(InEngineSubsystem);
}
