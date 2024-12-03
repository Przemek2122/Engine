#include "CoreEngine.h"
#include "Core/Interfaces/CoreLoop/ITickInterface.h"

#include "Engine/EngineTickingManager.h"

ITickInterface::ITickInterface()
	: bIsRegisteredTickInterface(false)
	, RegisteredPhase(ETickPhase::None)
	, TickFunctor(FFunctorObject(this, &ITickInterface::Tick))
{
}

ITickInterface::~ITickInterface()
{
	if (bIsRegisteredTickInterface)
	{
		GEngine->GetEngineTickingManager()->UnRegisterInterface(TickFunctor, RegisteredPhase);
	}
}

void ITickInterface::RegisterTickInterface()
{
	if (!bIsRegisteredTickInterface)
	{
		bIsRegisteredTickInterface = true;

		RegisteredPhase = GetTickPhase();

		GEngine->GetEngineTickingManager()->RegisterInterface(TickFunctor, RegisteredPhase);
	}
}
