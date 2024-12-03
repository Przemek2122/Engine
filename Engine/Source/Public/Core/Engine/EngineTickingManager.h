#pragma once

#include "CoreMinimal.h"

enum class ETickPhase : Uint8;

/**
 * This class is responsible for managing the tick of the engine
 */
class FEngineTickingManager
{
public:
	FEngineTickingManager();
	~FEngineTickingManager();

	void RegisterInterface(FFunctorObject<ITickInterface, void, float>& TickFunctor, const ETickPhase TickInterfacePhase);
	void UnRegisterInterface(FFunctorObject<ITickInterface, void, float>& TickFunctor, const ETickPhase TickInterfacePhase);

	/** This is the main tick function that will be called from the engine (CoreEngine.cpp */
	void EngineTick(const float DeltaTime);

	void Tick(const float DeltaTime);

	bool HasDelegateIndex(const ETickPhase Index);

protected:
	void EnsureDelegateAtIndexExists(const ETickPhase Index);

protected:
	CMap<ETickPhase, std::shared_ptr<FDelegateSafe<void, float>>> TickDelegateToIndex;

};
