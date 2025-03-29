// Created by Przemys³aw Wiewióra 2025

#pragma once
#include "CoreMinimal.h"

/** Base class for creating engine subsystem */
class IEngineSubsystemInterface
{
public:
	IEngineSubsystemInterface();
	virtual ~IEngineSubsystemInterface();

	virtual void Initialize();
	virtual void DeInitialize();

	virtual void Tick();

};
