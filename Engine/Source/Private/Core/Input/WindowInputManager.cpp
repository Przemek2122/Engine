// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "Core/Input/WindowInputManager.h"

#include "Input/EventHandler.h"

FWindowInputManager::FWindowInputManager(FWindow* InWindow)
	: Window(InWindow)
{
	EventHandler = GEngine->GetEventHandler();

	if (EventHandler != nullptr)
	{
		Init();
	}
	else
	{
		LOG_ERROR("Unable to get EventHandler.");
	}
}

void FWindowInputManager::Init()
{
	InitializeMouseDelegates();
	InitializeKeyBoardDelegates();
}

void FWindowInputManager::InitializeMouseDelegates()
{
	MouseDelegates.Init(EventHandler);

	std::shared_ptr<FIniObject> EngineInputIniObject = EventHandler->GetEngineInputIniObject();

	MouseDelegates.AddInput(EventHandler, EngineInputIniObject.get(), "SDL_MOUSE_MOVE");
	MouseDelegates.AddInput(EventHandler, EngineInputIniObject.get(), "SDL_MOUSE_BUTTON_LEFT");
	MouseDelegates.AddInput(EventHandler, EngineInputIniObject.get(), "SDL_MOUSE_BUTTON_MIDDLE");
	MouseDelegates.AddInput(EventHandler, EngineInputIniObject.get(), "SDL_MOUSE_BUTTON_RIGHT");
}

void FWindowInputManager::InitializeKeyBoardDelegates()
{
	KeyBoardDelegates.Init(EventHandler);

	KeyBoardDelegates.ButtonEscape = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);

	KeyBoardDelegates.Button0 = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.Button1 = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.Button2 = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.Button3 = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.Button4 = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.Button5 = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.Button6 = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.Button7 = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.Button8 = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.Button9 = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);

	KeyBoardDelegates.ButtonA = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.ButtonB = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.ButtonC = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.ButtonD = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.ButtonE = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	// [...] @TODO Add other basic buttons
	KeyBoardDelegates.ButtonW = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.ButtonS = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);

	KeyBoardDelegates.ButtonArrowUP = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.ButtonArrowDOWN = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.ButtonArrowRIGHT = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
	KeyBoardDelegates.ButtonArrowLEFT = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);

	KeyBoardDelegates.ButtonDELETE = FAutoDeletePointer<FInputDelegateWrapper>(EventHandler);
}
