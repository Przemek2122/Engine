// Created by Przemys³aw Wiewióra 2024

#pragma once

class FTimer;

/**
 * Optional params. Must not be created as null or timer will not start.
 * You can extend this struct to pass any needed parameters.
 */
struct FOptionalTimerParams
{
	FTimer* Timer;
};

/**
 * Global registry for timers.
 */
class FTimerCollector
{
public:
	FTimerCollector();

	static FTimerCollector* Get();

	void RegisterTimer(FTimer* Timer);
	void UnRegisterTimer(FTimer* Timer);

	void PauseAllTimers();
	void ResumeAllTimers();

	bool AreTimersPaused() const { return bAreTimersPaused; }

protected:
	/** All registered timers */
	CArray<FTimer*> Timers;
	CArray<FTimer*> PausedTimers;

	bool bAreTimersPaused;

};

/**
 * Timer class is used for managing raw sdl timer.
 * It has FOptionalTimerParams that will be passed to delegate on timer finish. if needed you can override it and pass custom
 */
class FTimer
{
public:
	/**
	 * Constructor using float. 1 MS equals 0.001.
	 * Note: Timer should not extend 49 days as single call.
	 */
	FTimer(FDelegateSafe<void, FOptionalTimerParams*>& InOnFinishDelegate, const float Time, std::shared_ptr<FOptionalTimerParams> InOptionalTimerParams = nullptr, bool bInRunAsyncOnFinish = true);

	/**
	 * Constructor using integer.
	 * Note: Timer should not extend 49 days as single call.
	 */
	FTimer(FDelegateSafe<void, FOptionalTimerParams*>& InOnFinishDelegate, const Uint32 Time, std::shared_ptr<FOptionalTimerParams> InOptionalTimerParams = nullptr, bool bInRunAsyncOnFinish = true);

	virtual ~FTimer();

	/** Called when timer finishes */
	static Uint32 OnTimerFinished(void* InOptionalTimerParams, SDL_TimerID InTimerID, Uint32 InInterval);

	void OnSynchronousTimerFinished();

	/** Start timer. Use when paused or when you want to make looping timer in your InOnFinishDelegate with bRestartTimer to start it again. */
	void StartTimer(const bool bRestartTimer = false);

	/** Pause timer and save time left. When you trigger StartTimer with bRestartTimer = false it will trigger timer with time remaining */
	void PauseTimer();

	bool IsActive() const;
	virtual bool IsPausableByTimerCollector();

	/** returns time left using conversion to float */
	float GetTimeLeft() const;

	/** @returns time left using variable - performance efficient */
	Uint32 GetTimeLeftRaw() const;

	Uint32 GetTimeMSElapsedSinceStart() const;

	/** @returns raw SDL Timer id */
	SDL_TimerID GetTimerId() const;

	FOptionalTimerParams* GetOptionalTimerParams() const;

	/** @returns percent of timer elapsed (In range of (0.f to 1.f where 1 is finished and 0 is just started) */
	float GetTimerPercent() const;

protected:
	/** Function used by constructors to run function */
	void InitializeTimer();

	/** Conversion from seconds (float) to milliseconds (Uint32) */
	static Uint32 TimeFloatToMs(const float Time);

	/** Conversion from milliseconds (Uint32) to seconds (float) */
	static float TimeMsToFloat(const Uint32 Time);

private:
	/** if true OnFinishDelegate will be called async */
	bool bRunAsyncOnFinish;

	/** True if running, false if paused */
	bool bIsTimerActive;

	/** Delegate called when  */
	FDelegateSafe<void, FOptionalTimerParams*> OnFinishDelegate;

	/** Optional parameters passed in with delegates */
	std::shared_ptr<FOptionalTimerParams> OptionalTimerParams;

	/** Time left of time set on begging or pause */
	Uint32 TimeLeftRaw;

	/** Initial time of timer. Set when starting timer. */
	Uint32 InitialTimerTime;

	/** Time since app start of when timer was started */
	Uint64 TimeStartOfTimer;

	SDL_TimerID TimerIDRaw;

};
