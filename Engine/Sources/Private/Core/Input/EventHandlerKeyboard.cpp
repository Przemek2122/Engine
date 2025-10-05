#include "CoreEngine.h"
#include "Input/EventHandlerKeyboard.h"
#include "Input/EventHandler.h"

void FInputDelegateWrapper::Execute(const EInputState InputState)
{
	if (!bWasSentAlready)
	{
		AddToResetQueue();

		if (EInputState::PRESS == InputState && CurrentInputState == EInputState::NOT_PRESSED)
		{
			bool bInputConsumed = false;

			const FFunctorLambda<bool, FFunctorBase<bool, EInputState>*, EInputState> Lambda(
			[&](FFunctorBase<bool, EInputState>* Function, EInputState) -> bool
			{
				if (!bInputConsumed)
				{
					const bool bFunctionResult = Function->operator()(InputState);
					if (bFunctionResult)
					{
						bInputConsumed = true;
					}
				}

				return bInputConsumed;
			});

			Delegate.ExecuteByLambda(Lambda, InputState);

			CurrentInputState = EInputState::PRESS;
		}
		else if (EInputState::RELEASE == InputState
			&& (CurrentInputState == EInputState::PRESS || CurrentInputState == EInputState::NOT_PRESSED))
		{
			Delegate.Execute(InputState);

			bWasSentAlready = true;
		}
	}
}

void FInputDelegateWrapper::Reset()
{
	bWasSentAlready = false;

	CurrentInputState = EInputState::NOT_PRESSED;
}

void FInputDelegateWrapper::AddToResetQueue()
{
	EventHandler->AddKeyboardInputDelegateToReset(this);
}

void FKeyBoardDelegates::Init(FEventHandler* EventHandler)
{
	const std::shared_ptr<FInputDelegateWrapper> InputPtr = std::make_shared<FInputDelegateWrapper>(EventHandler);

	RawInputNameToDelegateMap.Emplace(DefaultInputName, InputPtr);
	InputNameToDelegateMap.Emplace(DefaultInputName, InputPtr);
}

void FKeyBoardDelegates::AddInput(FEventHandler* EventHandler, const std::string& RawInputName, const std::string& InputName)
{
#if _DEBUG
	if (EventHandler != nullptr)
	{
#endif

		const std::shared_ptr<FInputDelegateWrapper> InputPtr = std::make_shared<FInputDelegateWrapper>(EventHandler);

		RawInputNameToDelegateMap.Emplace(RawInputName, InputPtr);
		InputNameToDelegateMap.Emplace(InputName, InputPtr);

#if _DEBUG
	}
	else
	{
		LOG_ERROR("EventHandler can not be nullptr!");
	}
#endif
}

FInputDelegateWrapper* FKeyBoardDelegates::GetMouseDelegateByName(const std::string& InputName)
{
	if (InputNameToDelegateMap.ContainsKey(InputName))
	{
		return InputNameToDelegateMap[InputName].get();
	}
	else
	{
		LOG_ERROR("FKeyBoardDelegates::GetMouseDelegateByName returns default input mapping. It will not work.");

		return InputNameToDelegateMap[DefaultInputName].get();
	}
}
