// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"
#include "EventHandlerKeyboard.h"
#include "EventHandlerMouse.h"

/**
 * Window input manager.
 * Separated from EventHandler because it does not know which window is used
 */
class FWindowInputManager
{
public:
	FWindowInputManager(FWindow* InWindow);

	void Init();

	FMouseDelegates MouseDelegates;
	FKeyBoardDelegates KeyBoardDelegates;

protected:
	void InitializeMouseDelegates();
	void InitializeKeyBoardDelegates();

protected:
	FWindow* Window;
	FEventHandler* EventHandler;

};
