#include "CoreEngine.h"
#include "Engine/EngineRenderingManager.h"

#include "Interfaces/CoreLoop/IRenderInterface.h"

FEngineRenderingManager::~FEngineRenderingManager()
{
	RenderDelegateToIndex.Clear();
}

void FEngineRenderingManager::RegisterInterface(FFunctorObject<IRenderInterface, void>& TickFunctor, const ERenderPhase TickInterfacePhase)
{
	if (TickFunctor.IsValid())
	{
		EnsureDelegateAtIndexExists(TickInterfacePhase);

		RenderDelegateToIndex[TickInterfacePhase]->BindObject(&TickFunctor);
	}
}

void FEngineRenderingManager::UnRegisterInterface(FFunctorObject<IRenderInterface, void>& TickFunctor, const ERenderPhase TickInterfacePhase)
{
	if (TickFunctor.IsValid())
	{
		if (RenderDelegateToIndex.ContainsKey(TickInterfacePhase))
		{
			RenderDelegateToIndex[TickInterfacePhase]->UnBindObject(&TickFunctor);
		}
		else
		{
			LOG_ERROR("Tried to unregister a delegate that does not have key.");
		}
	}
}

void FEngineRenderingManager::EngineRender()
{
	Render();
}

void FEngineRenderingManager::Render()
{
#if _DEBUG
	ERenderPhase LastIndex = ERenderPhase::None;
#endif

	for (auto& [Index, Delegate] : RenderDelegateToIndex)
	{
#if _DEBUG
		if (LastIndex > Index)
		{
			LOG_ERROR("We have incorrect setup of Rendering delegates in RenderingDelegateToIndex map.");
		}

		LastIndex = Index;
#endif

		Delegate->Execute();
	}
}

bool FEngineRenderingManager::HasDelegateIndex(const ERenderPhase Index)
{
	return RenderDelegateToIndex.ContainsKey(Index);
}

void FEngineRenderingManager::EnsureDelegateAtIndexExists(const ERenderPhase Index)
{
	if (!HasDelegateIndex(Index))
	{
		RenderDelegateToIndex.Emplace(Index, new FDelegate<void>());

		// Might need to sort to make sure it will be executed in the correct order
		//RenderDelegateToIndex.Sort()
	}
}
