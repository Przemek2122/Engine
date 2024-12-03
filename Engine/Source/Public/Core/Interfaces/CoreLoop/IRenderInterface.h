#pragma once

#include "CoreMinimal.h"

/**
 * Enum for render phases (defines order of rendering, first render first)
 */
enum class ERenderPhase : Uint8
{
	None = 0,				// This should not be ever used.
	Background,				// Special case for background rendering (map)
	Entities,				// Main game rendering		
	Default,				// Default rendering - Rendered in front of entities but behind widgets
	Widgets,				// Widgets rendering
	PostEverything = 254,	// Special case for objects that needs to be rendered last
	MAX = 255				// Max value. This should not be ever used.
};

/**
 * Class for objects that need to be ticked
 * Self registers and unregisters
 */
class IRenderInterface
{
public:
	IRenderInterface();
	virtual ~IRenderInterface();

	/** This function must be called when you want your object to start ticking. */
	void Register();

	/** @returns Phase registered in constructor. */
	ERenderPhase GetRegisteredPhase() const { return RegisteredPhase; }

	/** Called only once, so changing return value does not change state */
	virtual ERenderPhase GetRenderPhase() const = 0;

	/** Called every frame in given phase */
	virtual void Render() = 0;

private:
	bool bIsRegistered;
	ERenderPhase RegisteredPhase;
	FFunctorObject<IRenderInterface, void> RenderFunctor;
};
