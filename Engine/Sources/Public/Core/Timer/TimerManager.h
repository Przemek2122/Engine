// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "Timer.h"

/**
 * Class for managing timers.
 */
class FTimerManager
{
public:
	/**
	 * Call to create new timer that will be run on async timer thread on end.
	 * Note that this async thread is used for timer so using it too heavily can affect other timers.
	 *
	 * @param InOnFinishDelegate	Delegate to call when timer finishes
	 * @param Time					Time to trigger Delegate
	 * @param OptionalTimerParams	Optional params which will be transformed into shared_ptr, so you can use new and do not worry about memory
	 * @return						Timer shared_ptr which can be managed if needed (pause, start, etc...)
	 *
	 * @note						!! You have to save returned shared_ptr somewhere in your code or it will be immediately REMOVED. !!
	 */
	template<typename TOptionalTimerParams = FOptionalTimerParams>
	static std::shared_ptr<FTimer> CreateTimerAsync(FDelegateSafe<void, FOptionalTimerParams*>& InOnFinishDelegate, const float Time, TOptionalTimerParams* OptionalTimerParams = new TOptionalTimerParams())
	{
		std::shared_ptr<TOptionalTimerParams> OptionalTimerParamsPtr = std::make_shared<TOptionalTimerParams>();

		std::shared_ptr<FTimer> TimerPtr = std::make_shared<FTimer>(InOnFinishDelegate, Time, OptionalTimerParamsPtr, true);

		return TimerPtr;
	}

	/**
	 * Call to create new timer that will be run on begging of NEXT TICK after timer finishes.
	 *
	 * @param InOnFinishDelegate	Delegate to call when timer finishes
	 * @param Time					Time to trigger Delegate
	 * @param OptionalTimerParams	Optional params which will be transformed into shared_ptr, so you can use new and do not worry about memory
	 * @return						Timer shared_ptr which can be managed if needed (pause, start, etc...)
	 *
	 * @note						!! You have to save returned shared_ptr somewhere in your code or it will be immediately REMOVED. !!
	 */
	template<typename TOptionalTimerParams = FOptionalTimerParams>
	static std::shared_ptr<FTimer> CreateTimerSync(FDelegateSafe<void, FOptionalTimerParams*>& InOnFinishDelegate, const float Time, TOptionalTimerParams* OptionalTimerParams = new TOptionalTimerParams())
	{
		std::shared_ptr<TOptionalTimerParams> OptionalTimerParamsPtr = std::make_shared<TOptionalTimerParams>();

		std::shared_ptr<FTimer> TimerPtr = std::make_shared<FTimer>(InOnFinishDelegate, Time, OptionalTimerParamsPtr, false);

		return TimerPtr;
	}

	/**
	 * Call to create new timer that will be run on async timer thread on end.
	 * Note that this async thread is used for timer so using it too heavily can affect other timers.
	 *
	 * @param Lambda				Lambda to call when timer finishes
	 * @param Time					Time to trigger Delegate
	 * @param OptionalTimerParams	Optional params which will be transformed into shared_ptr, so you can use new and do not worry about memory
	 * @return						Timer shared_ptr which can be managed if needed (pause, start, etc...)
	 *
	 * @note						!! You have to save returned shared_ptr somewhere in your code or it will be immediately REMOVED. !!
	 */
	template<typename TOptionalTimerParams = FOptionalTimerParams, typename TAutoLambdaType>
	static std::shared_ptr<FTimer> CreateTimerAsync(TAutoLambdaType Lambda, const float Time, TOptionalTimerParams* OptionalTimerParams = new TOptionalTimerParams())
	{
		FDelegateSafe<void, FOptionalTimerParams*> OnFinishDelegate;
		OnFinishDelegate.BindLambda(Lambda);

		std::shared_ptr<TOptionalTimerParams> OptionalTimerParamsPtr = std::make_shared<TOptionalTimerParams>();

		std::shared_ptr<FTimer> TimerPtr = std::make_shared<FTimer>(OnFinishDelegate, Time, OptionalTimerParamsPtr, true);

		return TimerPtr;
	}

	/**
	 * Call to create new timer that will be run on async timer thread on end.
	 * Note that this async thread is used for timer so using it too heavily can affect other timers.
	 *
	 * @param Lambda				Lambda to call when timer finishes
	 * @param Time					Time to trigger Delegate
	 * @param OptionalTimerParams	Optional params which will be transformed into shared_ptr, so you can use new and do not worry about memory
	 * @return						Timer shared_ptr which can be managed if needed (pause, start, etc...)
	 *
	 * @note						!! You have to save returned shared_ptr somewhere in your code or it will be immediately REMOVED. !!
	 */
	template<typename TOptionalTimerParams = FOptionalTimerParams, typename TAutoLambdaType>
	static std::shared_ptr<FTimer> CreateTimerSync(TAutoLambdaType Lambda, const float Time, TOptionalTimerParams* OptionalTimerParams = new TOptionalTimerParams())
	{
		FDelegateSafe<void, FOptionalTimerParams*> OnFinishDelegate;
		OnFinishDelegate.BindLambda(Lambda);

		std::shared_ptr<TOptionalTimerParams> OptionalTimerParamsPtr = std::make_shared<TOptionalTimerParams>();

		std::shared_ptr<FTimer> TimerPtr = std::make_shared<FTimer>(OnFinishDelegate, Time, OptionalTimerParamsPtr, false);

		return TimerPtr;
	}

};
