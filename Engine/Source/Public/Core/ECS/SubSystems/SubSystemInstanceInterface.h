// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

class ISubSystemManagerInterface;

/**
 * Interface for any object which should be subsystem
 */
class ISubSystemInstanceInterface
{
public:
	ISubSystemInstanceInterface();
	virtual ~ISubSystemInstanceInterface();

	void SetSubSystemManager(ISubSystemManagerInterface* InSubSystemParentInterface, const bool bAutoInit = true);

	virtual void InitializeSubSystem();
	virtual void TickSubSystem();
	virtual void RenderSubSystem();

	ISubSystemManagerInterface* GetSubSystemParentInterface() const { return SubSystemParentInterface; }

private:
	ISubSystemManagerInterface* SubSystemParentInterface;

};
