// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "Assets/IniReader/IniObject.h"
#include "Core/Input/EventHandlerMouse.h"
#include "Core/Input/EventHandlerKeyboard.h"

static const std::string DefaultInputName = "Default";

class FEventHandler
{
public:
	FEventHandler(const SDL_Event& InEvent);
	virtual ~FEventHandler();

	void HandleEvents();

	void ResetAll();

	void InitializeInputFromConfig();

	_NODISCARD bool QuitInputDetected() const;

	_NODISCARD bool HasMouseMoved() const;
	_NODISCARD FVector2D<int> GetMouseLocationCurrent() const;
	_NODISCARD FVector2D<int> GetMouseLocationLast() const;

	std::shared_ptr<FIniObject> GetEngineInputIniObject() const;

	void AddMouseInputDelegateToReset(FMouseInputDelegateWrapper* MouseInputDelegateWrapper);
	void AddKeyboardInputDelegateToReset(FInputDelegateWrapper* KeyboardInputDelegateWrapper);

	/**
	 * Perhaps create an array with SDL KeyCodes? and binding them like
	 * BindEvent(SDL_MOUSEMOTION, this, &ECameraManager::OnMouseMove);
	 */

protected:
	virtual void SwitchOnInput(Uint32 EventType);

	virtual void InputKeyDown();
	virtual void InputKeyUp();
	virtual void MouseMotion();
	virtual void InputMouseDown();
	virtual void InputMouseUp();
	virtual void InputWindowEvent();

	FWindow* GetCurrentlyFocusedWindow() const;

protected:
	/** This map is used to reset mouse delegates. */
	CArray<FMouseInputDelegateWrapper*> MouseInputDelegateResetQueue;
	/** This map is used to reset keyboard delegates. */
	CArray<FInputDelegateWrapper*> KeyboardInputDelegateResetQueue;

	FVector2D<int> MouseLocationCurrent;
	FVector2D<int> MouseLocationLast;

	SDL_Event Event;
	
	bool bQuitInputDetected;

	std::shared_ptr<FIniObject> EngineInputIniObject;
	
};
