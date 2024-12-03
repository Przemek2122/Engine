#pragma once

#include "CoreMinimal.h"

/**
 * Enum for tick phases (defines order of ticking, first tick first)
 */
enum class ETickPhase : Uint8
{
	None = 0,					// Not used
	PostEngine,					// Called right after engine tick
	Background,					// Called for background rendering callculations
	Entity,						// Called for entities	
	Default,					// Called for everything else
	Widget,						// Called for widgets
	PostEverything = 254,		// Called after everything else
	MAX = 255					// Max value
};

/**
 * Class for objects that need to be ticked
 * Self registers and unregisters
 */
class ITickInterface
{
public:
	ITickInterface();
	virtual ~ITickInterface();

	/** This function must be called when you want your object to start ticking. */
	void RegisterTickInterface();

	/** @returns Phase registered in constructor. */
	ETickPhase GetRegisteredPhase() const { return RegisteredPhase; }

	/** Called only once, so changing return value does not change state */
	virtual ETickPhase GetTickPhase() const = 0;

	/** Called every frame in given phase */
	virtual void Tick(float DeltaTime) = 0;

private:
	bool bIsRegisteredTickInterface;
	ETickPhase RegisteredPhase;
	FFunctorObject<ITickInterface, void, float> TickFunctor;

};
