// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Input/EventHandler.h"

#include "Assets/IniReader/IniManager.h"
#include "Assets/IniReader/IniObject.h"
#include "Input/WindowInputManager.h"

FEventHandler::FEventHandler(const SDL_Event& InEvent)
	: Event(InEvent)
	, bQuitInputDetected(false)
{
}

FEventHandler::~FEventHandler()
{
	MouseInputDelegateResetQueue.Clear();
	KeyboardInputDelegateResetQueue.Clear();
}

void FEventHandler::HandleEvents()
{
	ResetAll();

	// @TODO SDLK_1 to '1' - Performance Improvement
	
	while (SDL_PollEvent(&Event)) 
	{
		const Uint32 EventType = Event.type;
		
		SwitchOnInput(EventType);
	}
}

void FEventHandler::ResetAll()
{
	for (FMouseInputDelegateWrapper* InputDelegate : MouseInputDelegateResetQueue)
	{
		InputDelegate->Reset();
	}

	MouseInputDelegateResetQueue.Clear();

	for (FInputDelegateWrapper* InputDelegate : KeyboardInputDelegateResetQueue)
	{
		InputDelegate->Reset();
	}

	KeyboardInputDelegateResetQueue.Clear();
}

void FEventHandler::InitializeInputFromConfig()
{
	const std::string EngineInputSettingsIniName = "EngineInput";
	FIniManager* IniManager = GEngine->GetAssetsManager()->GetIniManager();
	EngineInputIniObject = IniManager->GetIniObject(EngineInputSettingsIniName);
	if (EngineInputIniObject->DoesIniExist())
	{
		EngineInputIniObject->LoadIni();
	}
}

bool FEventHandler::QuitInputDetected() const
{
	return bQuitInputDetected;
}

bool FEventHandler::HasMouseMoved() const
{
	return MouseLocationCurrent != MouseLocationLast;
}

FVector2D<int> FEventHandler::GetMouseLocationCurrent() const
{
	return MouseLocationCurrent;
}

FVector2D<int> FEventHandler::GetMouseLocationLast() const
{
	return MouseLocationLast;
}

std::shared_ptr<FIniObject> FEventHandler::GetEngineInputIniObject() const
{
	return EngineInputIniObject;
}

void FEventHandler::AddMouseInputDelegateToReset(FMouseInputDelegateWrapper* MouseInputDelegateWrapper)
{
	MouseInputDelegateResetQueue.Push(MouseInputDelegateWrapper);
}

void FEventHandler::AddKeyboardInputDelegateToReset(FInputDelegateWrapper* KeyboardInputDelegateWrapper)
{
	KeyboardInputDelegateResetQueue.Push(KeyboardInputDelegateWrapper);
}

void FEventHandler::SwitchOnInput(const Uint32 EventType)
{
	InputWindowEvent();

	switch (EventType)
	{
		/** User requested quit */
		case SDL_EVENT_QUIT:
		{
			bQuitInputDetected = true;

			LOG_DEBUG("Quit input.");

			break;
		}

		/** Keyboard */
		case SDL_EVENT_KEY_DOWN:
		{
			InputKeyDown();

			break;
		}

		case SDL_EVENT_KEY_UP:
		{
			InputKeyUp();

			break;
		}

		/** Mouse movement X & Y */
		case SDL_EVENT_MOUSE_MOTION:
		{
			MouseMotion();

			break;
		}

		/** Mouse buttons down */
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		{
			InputMouseDown();

			break;
		}

		/** Mouse buttons release */
		case SDL_EVENT_MOUSE_BUTTON_UP:
		{
			InputMouseUp();

			break;
		}

		default:
		{
			LOG_DEBUG("Unknown input found. Input: " << Event.type);
		}
	}
}

void FEventHandler::InputKeyDown()
{
	// Sent input only to focused window
	if (FWindow* CurrentWindow = GetCurrentlyFocusedWindow())
	{
		FWindowInputManager* CurrentWindowInputManger = CurrentWindow->GetWindowInputManager();
		FKeyBoardDelegates& KeyBoardDelegates = CurrentWindowInputManger->KeyBoardDelegates;

		switch (Event.key.key)
		{
			case SDLK_ESCAPE:
			{
				KeyBoardDelegates.ButtonEscape->Execute(EInputState::PRESS);

				break;
			}

			case SDLK_0:
			{
				KeyBoardDelegates.Button0->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_1:
			{
				KeyBoardDelegates.Button1->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_2:
			{
				KeyBoardDelegates.Button2->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_3:
			{
				KeyBoardDelegates.Button3->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_4:
			{
				KeyBoardDelegates.Button4->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_5:
			{
				KeyBoardDelegates.Button5->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_6:
			{
				KeyBoardDelegates.Button6->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_7:
			{
				KeyBoardDelegates.Button7->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_8:
			{
				KeyBoardDelegates.Button8->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_9:
			{
				KeyBoardDelegates.Button9->Execute(EInputState::PRESS);

				break;
			}

			case SDLK_UP:
			{
				KeyBoardDelegates.ButtonArrowUP->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_DOWN:
			{
				KeyBoardDelegates.ButtonArrowDOWN->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_RIGHT:
			{
				KeyBoardDelegates.ButtonArrowRIGHT->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_LEFT:
			{
				KeyBoardDelegates.ButtonArrowLEFT->Execute(EInputState::PRESS);

				break;
			}

			case SDLK_DELETE:
			{
				KeyBoardDelegates.ButtonDELETE->Execute(EInputState::PRESS);

				break;
			}

			case SDLK_A:
			{
				KeyBoardDelegates.ButtonA->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_B:
			{
				KeyBoardDelegates.ButtonB->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_C:
			{
				KeyBoardDelegates.ButtonC->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_D:
			{
				KeyBoardDelegates.ButtonD->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_E:
			{
				KeyBoardDelegates.ButtonE->Execute(EInputState::PRESS);

				break;
			}
			// [...] @TODO Someday all add buttons
			case SDLK_W:
			{
				KeyBoardDelegates.ButtonW->Execute(EInputState::PRESS);

				break;
			}
			case SDLK_S:
			{
				KeyBoardDelegates.ButtonS->Execute(EInputState::PRESS);

				break;
			}

			default:
			{
				LOG_DEBUG("Unknown keyboard input found.");
			}
		}
	}
}

void FEventHandler::InputKeyUp()
{
	// Sent input only to focused window
	if (FWindow* CurrentWindow = GetCurrentlyFocusedWindow())
	{
		FWindowInputManager* CurrentWindowInputManger = CurrentWindow->GetWindowInputManager();
		FKeyBoardDelegates& KeyBoardDelegates = CurrentWindowInputManger->KeyBoardDelegates;

		switch (Event.key.key)
		{
			case SDLK_ESCAPE:
			{
				KeyBoardDelegates.ButtonEscape->Execute(EInputState::RELEASE);

				break;
			}

			case SDLK_0:
			{
				KeyBoardDelegates.Button0->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_1:
			{
				KeyBoardDelegates.Button1->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_2:
			{
				KeyBoardDelegates.Button2->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_3:
			{
				KeyBoardDelegates.Button3->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_4:
			{
				KeyBoardDelegates.Button4->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_5:
			{
				KeyBoardDelegates.Button5->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_6:
			{
				KeyBoardDelegates.Button6->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_7:
			{
				KeyBoardDelegates.Button7->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_8:
			{
				KeyBoardDelegates.Button8->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_9:
			{
				KeyBoardDelegates.Button9->Execute(EInputState::RELEASE);

				break;
			}

			case SDLK_UP:
			{
				KeyBoardDelegates.ButtonArrowUP->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_DOWN:
			{
				KeyBoardDelegates.ButtonArrowDOWN->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_RIGHT:
			{
				KeyBoardDelegates.ButtonArrowRIGHT->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_LEFT:
			{
				KeyBoardDelegates.ButtonArrowLEFT->Execute(EInputState::RELEASE);

				break;
			}

			case SDLK_DELETE:
			{
				KeyBoardDelegates.ButtonDELETE->Execute(EInputState::RELEASE);

				break;
			}

			case SDLK_A:
			{
				KeyBoardDelegates.ButtonA->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_B:
			{
				KeyBoardDelegates.ButtonB->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_C:
			{
				KeyBoardDelegates.ButtonC->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_D:
			{
				KeyBoardDelegates.ButtonD->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_E:
			{
				KeyBoardDelegates.ButtonE->Execute(EInputState::RELEASE);

				break;
			}
			// [...] @TODO Someday all add buttons
			case SDLK_W:
			{
				KeyBoardDelegates.ButtonW->Execute(EInputState::RELEASE);

				break;
			}
			case SDLK_S:
			{
				KeyBoardDelegates.ButtonS->Execute(EInputState::RELEASE);

				break;
			}

			default:
			{
				LOG_DEBUG("Unknown keyboard input found.");
			}
		}
	}
}

void FEventHandler::MouseMotion()
{
	// Sent input only to focused window
	if (FWindow* CurrentWindow = GetCurrentlyFocusedWindow())
	{
		if (MouseLocationLast != MouseLocationCurrent)
		{
			FWindowInputManager* CurrentWindowInputManger = CurrentWindow->GetWindowInputManager();
			FMouseDelegates& MouseDelegates = CurrentWindowInputManger->MouseDelegates;

			MouseLocationLast = MouseLocationCurrent;

			MouseDelegates.GetMouseDelegateByNameRaw("SDL_MOUSE_MOVE")->Execute(MouseLocationCurrent, EInputState::PRESS);
		}

		MouseLocationCurrent.X = Event.motion.x;
		MouseLocationCurrent.Y = Event.motion.y;
	}
}

void FEventHandler::InputMouseDown()
{
	// Sent input only to focused window
	if (FWindow* CurrentWindow = GetCurrentlyFocusedWindow())
	{
		FWindowInputManager* CurrentWindowInputManger = CurrentWindow->GetWindowInputManager();
		FMouseDelegates& MouseDelegates = CurrentWindowInputManger->MouseDelegates;

		switch (Event.button.button)
		{
			case SDL_BUTTON_LEFT:
			{
				MouseDelegates.GetMouseDelegateByNameRaw("SDL_MOUSE_BUTTON_LEFT")->Execute(MouseLocationCurrent, EInputState::PRESS);

				break;
			}

			case SDL_BUTTON_MIDDLE:
			{
				MouseDelegates.GetMouseDelegateByNameRaw("SDL_MOUSE_BUTTON_MIDDLE")->Execute(MouseLocationCurrent, EInputState::PRESS);

				break;
			}

			case SDL_BUTTON_RIGHT:
			{
				MouseDelegates.GetMouseDelegateByNameRaw("SDL_MOUSE_BUTTON_RIGHT")->Execute(MouseLocationCurrent, EInputState::PRESS);

				break;
			}

			default:
			{
				LOG_DEBUG("Unknown mouse input found.");
			}
		}
	}
}

void FEventHandler::InputMouseUp()
{
	// Sent input only to focused window
	if (FWindow* CurrentWindow = GetCurrentlyFocusedWindow())
	{
		FWindowInputManager* CurrentWindowInputManger = CurrentWindow->GetWindowInputManager();
		FMouseDelegates& MouseDelegates = CurrentWindowInputManger->MouseDelegates;

		switch (Event.button.button)
		{
			case SDL_BUTTON_LEFT:
			{
				MouseDelegates.GetMouseDelegateByNameRaw("SDL_MOUSE_BUTTON_LEFT")->Execute(MouseLocationCurrent, EInputState::RELEASE);

				break;
			}

			case SDL_BUTTON_MIDDLE:
			{
				MouseDelegates.GetMouseDelegateByNameRaw("SDL_MOUSE_BUTTON_MIDDLE")->Execute(MouseLocationCurrent, EInputState::RELEASE);

				break;
			}

			case SDL_BUTTON_RIGHT:
			{
				MouseDelegates.GetMouseDelegateByNameRaw("SDL_MOUSE_BUTTON_RIGHT")->Execute(MouseLocationCurrent, EInputState::RELEASE);

				break;
			}

			default:
			{
				LOG_DEBUG("Unknown mouse input found.");
			}
		}
	}
}

void FEventHandler::InputWindowEvent()
{
	switch (Event.window.type)
	{
		case SDL_EVENT_WINDOW_SHOWN:
		{
			LOG_DEBUG("Window " << Event.window.windowID << " shown");
			break;
		}

		case SDL_EVENT_WINDOW_HIDDEN:
		{
			LOG_DEBUG("Window " << Event.window.windowID << " hidden");
			GEngine->GetEngineRender()->OnWindowHidden(Event.window.windowID);
			break;
		}

		case SDL_EVENT_WINDOW_EXPOSED:
		{
			LOG_DEBUG("Window " << Event.window.windowID << " exposed");
			GEngine->GetEngineRender()->OnWindowExposed(Event.window.windowID);
			break;
		}

		case SDL_EVENT_WINDOW_MOVED:
		{
			GEngine->GetEngineRender()->OnWindowMoved(Event.window.windowID, Event.window.data1, Event.window.data2);
			break;
		}

		case SDL_EVENT_WINDOW_RESIZED:
		{
			GEngine->GetEngineRender()->OnWindowResized(Event.window.windowID, Event.window.data1, Event.window.data2);
			break;
		}

		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
		{
			GEngine->GetEngineRender()->OnWindowSizeChanged(Event.window.windowID, Event.window.data1, Event.window.data2);
			break;
		}

		case SDL_EVENT_WINDOW_MINIMIZED:
		{
			GEngine->GetEngineRender()->OnWindowMinimized(Event.window.windowID);
			break;
		}

		case SDL_EVENT_WINDOW_MAXIMIZED:
		{
			GEngine->GetEngineRender()->OnWindowMaximized(Event.window.windowID);
			break;
		}

		case SDL_EVENT_WINDOW_RESTORED:
		{
			LOG_DEBUG("Window " << Event.window.windowID << " restored.");
			break;
		}

		case SDL_EVENT_WINDOW_MOUSE_ENTER:
		{
			GEngine->GetEngineRender()->SetWindowIsMouseInside(Event.window.windowID, true);
			break;
		}

		case SDL_EVENT_WINDOW_MOUSE_LEAVE:
		{
			GEngine->GetEngineRender()->SetWindowIsMouseInside(Event.window.windowID, false);
			break;
		}

		case SDL_EVENT_WINDOW_FOCUS_GAINED:
		{
			GEngine->GetEngineRender()->SetWindowFocus(Event.window.windowID, true);
			break;
		}

		case SDL_EVENT_WINDOW_FOCUS_LOST:
		{
			GEngine->GetEngineRender()->SetWindowFocus(Event.window.windowID, false);
			break;
		}

		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
		{
			LOG_DEBUG("Window " << Event.window.windowID << " closed");
			break;
		}

		default:
		{
			LOG_DEBUG("Window " << Event.window.windowID << " got unknown event" << Event.window.type);
		}
	}
}

FWindow* FEventHandler::GetCurrentlyFocusedWindow() const
{
	return GEngine->GetEngineRender()->GetFocusedWindow();
}
