#include "CoreEngine.h"
#include "Input/EventHandlerMouse.h"
#include "Input/EventHandler.h"

void FMouseInputDelegateWrapper::Execute(const FVector2D<int>& Location, const EInputState InputState)
{
	if (!bWasSentAlready)
	{
		AddToResetQueue();

		bool bWasInputConsumed = false;

		auto ExecutionLambda = [&bWasInputConsumed](FFunctorBase<bool, FVector2D<int>, EInputState>* Function, FVector2D<int> LambdaLocation, EInputState LambdaInputState) -> bool
		{
			if (!bWasInputConsumed)
			{
				const bool bShouldConsumeInput = Function->operator()(LambdaLocation, LambdaInputState);

				if (bShouldConsumeInput)
				{
					bWasInputConsumed = bShouldConsumeInput;

					return bShouldConsumeInput;
				}
			}

			return false;
		};

		if (InputState == EInputState::PRESS && CurrentInputState == EInputState::NOT_PRESSED)
		{
			Delegate.ExecuteByLambda(ExecutionLambda, Location, InputState);

			CurrentInputState = EInputState::PRESS;
		}
		else if (InputState == EInputState::RELEASE && (CurrentInputState == EInputState::PRESS || CurrentInputState == EInputState::NOT_PRESSED))
		{
			Delegate.ExecuteByLambda(ExecutionLambda, Location, InputState);

			bWasSentAlready = true;
		}
	}
}

void FMouseInputDelegateWrapper::AddToResetQueue()
{
	EventHandler->AddMouseInputDelegateToReset(this);
}

void FMouseInputDelegateWrapper::Reset()
{
	bWasSentAlready = false;

	CurrentInputState = EInputState::NOT_PRESSED;
}

void FMouseDelegates::Init(FEventHandler* EventHandler)
{
	const std::shared_ptr<FMouseInputDelegateWrapper> InputPtr = std::make_shared<FMouseInputDelegateWrapper>(EventHandler);

	RawInputNameToDelegateMap.Emplace(DefaultInputName, InputPtr);
	InputNameToDelegateMap.Emplace(DefaultInputName, InputPtr);
}

void FMouseDelegates::AddInput(FEventHandler* EventHandler, FIniObject* InIniObject, const std::string& RawInputName)
{
#if _DEBUG
	if (EventHandler != nullptr)
	{
#endif

		if (InIniObject != nullptr)
		{
			FIniField IniField = InIniObject->FindFieldByName(RawInputName);
			if (IniField.IsValid())
			{
				const std::shared_ptr<FMouseInputDelegateWrapper> InputPtr = std::make_shared<FMouseInputDelegateWrapper>(EventHandler);

				RawInputNameToDelegateMap.Emplace(RawInputName, InputPtr);
				InputNameToDelegateMap.Emplace(IniField.GetValueAsString(), InputPtr);
			}
		}

#if _DEBUG
	}
	else
	{
		LOG_ERROR("EventHandler can not be nullptr!");
	}
#endif
}

FMouseInputDelegateWrapper* FMouseDelegates::GetMouseDelegateByNameRaw(const std::string& InputName)
{
	if (RawInputNameToDelegateMap.ContainsKey(InputName))
	{
		return RawInputNameToDelegateMap[InputName].get();
	}
	else
	{
		LOG_ERROR("FMouseDelegates::GetMouseDelegateByNameRaw returns default input mapping. Input not found: " << InputName);

		return RawInputNameToDelegateMap[DefaultInputName].get();
	}
}

FMouseInputDelegateWrapper* FMouseDelegates::GetMouseDelegateByName(const std::string& InputName)
{
	if (InputNameToDelegateMap.ContainsKey(InputName))
	{
		return InputNameToDelegateMap[InputName].get();
	}
	else
	{
		LOG_ERROR("FMouseDelegates::GetMouseDelegateByName returns default input mapping. Input not found: " << InputName);

		return InputNameToDelegateMap[DefaultInputName].get();
	}
}
