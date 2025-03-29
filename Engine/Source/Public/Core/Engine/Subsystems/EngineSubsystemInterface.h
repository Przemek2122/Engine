// Created by Przemys�aw Wiewi�ra 2025

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
