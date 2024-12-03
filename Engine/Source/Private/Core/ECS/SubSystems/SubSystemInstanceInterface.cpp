// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "ECS/SubSystems/SubSystemInstanceInterface.h"

ISubSystemInstanceInterface::ISubSystemInstanceInterface()
	: SubSystemParentInterface(nullptr)
{
}

ISubSystemInstanceInterface::~ISubSystemInstanceInterface()
{
	if (SubSystemParentInterface != nullptr)
	{
		SubSystemParentInterface->UnRegisterSubSystem(this);
	}
}

void ISubSystemInstanceInterface::SetSubSystemManager(ISubSystemManagerInterface* InSubSystemParentInterface, const bool bAutoInit)
{
#if _DEBUG
	if (InSubSystemParentInterface != nullptr && SubSystemParentInterface == nullptr)
	{
#endif
		SubSystemParentInterface = InSubSystemParentInterface;
		SubSystemParentInterface->RegisterSubSystem(this);

		if (bAutoInit)
		{
			InitializeSubSystem();
		}
#if _DEBUG
	}
	else if (SubSystemParentInterface != nullptr)
	{
		LOG_ERROR("SubSystemParentInterface is already set.");
	}
	else
	{
		LOG_ERROR("InSubSystemParentInterface is NULL.");
	}
#endif
}

void ISubSystemInstanceInterface::InitializeSubSystem()
{
}

void ISubSystemInstanceInterface::TickSubSystem()
{
}

void ISubSystemInstanceInterface::RenderSubSystem()
{
}
