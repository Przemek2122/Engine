#include "CoreEngine.h"
#include "Engine/EngineTickingManager.h"

#include <memory>

#include "Interfaces/CoreLoop/ITickInterface.h"

FEngineTickingManager::FEngineTickingManager()
{
}

FEngineTickingManager::~FEngineTickingManager()
{
	TickDelegateToIndex.Clear();
}

void FEngineTickingManager::RegisterInterface(FFunctorObject<ITickInterface, void, float>& TickFunctor, const ETickPhase TickInterfacePhase)
{
	if (TickFunctor.IsValid())
	{
		EnsureDelegateAtIndexExists(TickInterfacePhase);

		TickDelegateToIndex[TickInterfacePhase]->BindObject(TickFunctor);
	}
	else
	{
		LOG_ERROR("Tried to register a delegate that is not valid.");
	}
}

void FEngineTickingManager::UnRegisterInterface(FFunctorObject<ITickInterface, void, float>& TickFunctor,const ETickPhase TickInterfacePhase)
{
	if (TickFunctor.IsValid())
	{
		if (TickDelegateToIndex.ContainsKey(TickInterfacePhase))
		{
			TickDelegateToIndex[TickInterfacePhase]->UnBindObject(TickFunctor);
		}
		else
		{
			LOG_ERROR("Tried to unregister a delegate that does not have key.");
		}
	}
	else
	{
		LOG_ERROR("Tried to unregister a delegate that is not valid.");
	}
}

void FEngineTickingManager::EngineTick(const float DeltaTime)
{
	Tick(DeltaTime);
}

void FEngineTickingManager::Tick(const float DeltaTime)
{
#if _DEBUG
	ETickPhase LastIndex = ETickPhase::None;
#endif

	for (auto& [Index, Delegate] : TickDelegateToIndex)
	{
#if _DEBUG
		if (LastIndex > Index)
		{
			LOG_ERROR("We have incorrect setup of Tick delegates in TickDelegateToIndex map.");
		}

		LastIndex = Index;
#endif

		Delegate->Execute(DeltaTime);
	}
}

bool FEngineTickingManager::HasDelegateIndex(const ETickPhase Index)
{
	return TickDelegateToIndex.ContainsKey(Index);
}

void FEngineTickingManager::EnsureDelegateAtIndexExists(const ETickPhase Index)
{
	if (!HasDelegateIndex(Index))
	{
		// This has issue
		TickDelegateToIndex.Emplace(Index, std::make_shared<FDelegateSafe<void, float>>());

		// Might need to sort to make sure it will be executed in the correct order
		//TickDelegateToIndex.Sort()
	}
}
