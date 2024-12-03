#pragma once

#include "CoreMinimal.h"
#include "Core/Input/EventHandlerShared.h"

/**
 * This class is used to handle keyboard events. It exists to allow rebinding.
 * If delegate returns true it means input is consumed and should not be sent anymore
 */
class FInputDelegateWrapper
{
public:
	FInputDelegateWrapper(FEventHandler* InEventHandler)
		: bWasSentAlready(false)
		, EventHandler(InEventHandler)
		, CurrentInputState(EInputState::NOT_PRESSED)
	{
	}

	void Execute(EInputState InputState);

	void Reset();

	/** Called when input is detected. */
	FDelegate<bool, EInputState> Delegate;

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
 * Keyboard delegates.
 * @Note Remember to initialize all delegates.
 */
class FKeyBoardDelegates
{
	typedef FAutoDeletePointer<FInputDelegateWrapper> FKeyBoardButtonDelegate;

public:
	void Init(FEventHandler* EventHandler);

	void AddInput(FEventHandler* EventHandler, const std::string& RawInputName, const std::string& InputName);

	FInputDelegateWrapper* GetMouseDelegateByName(const std::string& InputName);

	/** Called when escape is used */
	FKeyBoardButtonDelegate ButtonEscape;

	/** Called when '0' key is used */
	FKeyBoardButtonDelegate Button0;
	/** Called when '1' key is used */
	FKeyBoardButtonDelegate Button1;
	/** Called when '2' key is used */
	FKeyBoardButtonDelegate Button2;
	/** Called when '3' key is used */
	FKeyBoardButtonDelegate Button3;
	/** Called when '4' key is used */
	FKeyBoardButtonDelegate Button4;
	/** Called when '5' key is used */
	FKeyBoardButtonDelegate Button5;
	/** Called when '6' key is used */
	FKeyBoardButtonDelegate Button6;
	/** Called when '7' key is used */
	FKeyBoardButtonDelegate Button7;
	/** Called when '8' key is used */
	FKeyBoardButtonDelegate Button8;
	/** Called when '9' key is used */
	FKeyBoardButtonDelegate Button9;

	/** Called when 'A' key is used */
	FKeyBoardButtonDelegate ButtonA;
	/** Called when 'B' key is used */
	FKeyBoardButtonDelegate ButtonB;
	/** Called when 'C' key is used */
	FKeyBoardButtonDelegate ButtonC;
	/** Called when 'D' key is used */
	FKeyBoardButtonDelegate ButtonD;
	/** Called when 'E' key is used */
	FKeyBoardButtonDelegate ButtonE;

	/** Called when 'W' key is used */
	FKeyBoardButtonDelegate ButtonW;
	/** Called when 'S' key is used */
	FKeyBoardButtonDelegate ButtonS;

	/** Called when up arrow key is used */
	FKeyBoardButtonDelegate ButtonArrowUP;
	/** Called when down arrow key is used */
	FKeyBoardButtonDelegate ButtonArrowDOWN;
	/** Called when right arrow key is used */
	FKeyBoardButtonDelegate ButtonArrowRIGHT;
	/** Called when left arrow key is used */
	FKeyBoardButtonDelegate ButtonArrowLEFT;

	/** Called when 'Delete' key is used */
	FKeyBoardButtonDelegate ButtonDELETE;

protected:
	CMap<std::string, std::shared_ptr<FInputDelegateWrapper>> RawInputNameToDelegateMap;
	CMap<std::string, std::shared_ptr<FInputDelegateWrapper>> InputNameToDelegateMap;

};
