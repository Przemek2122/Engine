#pragma once

#include "CoreMinimal.h"
#include "Core/Input/EventHandlerShared.h"

class FIniObject;

/**
 * This class is used to handle mouse events.
 * If delegate returns true it means input is consumed and should not be sent anymore
 */
class FMouseInputDelegateWrapper
{
public:
	FMouseInputDelegateWrapper(FEventHandler* InEventHandler)
		: bWasSentAlready(false)
		, EventHandler(InEventHandler)
		, CurrentInputState(EInputState::NOT_PRESSED)
	{
	}

	void Execute(const FVector2D<int>& Location, const EInputState InputState);

	void Reset();

	FDelegate<bool, FVector2D<int>, EInputState> Delegate;

private:
	void AddToResetQueue();

private:
	/** True if input was sent already. */
	bool bWasSentAlready;

	FEventHandler* EventHandler;

	/** Current state of input. It is incremented in execute and then reset in reset */
	EInputState CurrentInputState;
};

/**
 * Mouse delegates.
 * @Note Remember to initialize all delegates.
 */
class FMouseDelegates
{
public:
	void Init(FEventHandler* EventHandler);

	/** Add input. RawInputName is for SDL codes while InputName is for our code use. */
	void AddInput(FEventHandler* EventHandler, FIniObject* InIniObject, const std::string& RawInputName);

	/** Raw engine internal use name */
	FMouseInputDelegateWrapper* GetMouseDelegateByNameRaw(const std::string& InputName);

	FMouseInputDelegateWrapper* GetMouseDelegateByName(const std::string& InputName);

protected:
	CMap<std::string, std::shared_ptr<FMouseInputDelegateWrapper>> RawInputNameToDelegateMap;
	CMap<std::string, std::shared_ptr<FMouseInputDelegateWrapper>> InputNameToDelegateMap;

};
