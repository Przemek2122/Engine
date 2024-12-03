// Created by Przemys³aw Wiewióra 2020-2024 https://github.com/Przemek2122/GameEngine

#pragma once

#include "CoreMinimal.h"
#include "Includes/EngineErrorCodes.h"
#include "Includes/EngineLaunchParameterCollection.h"

enum class EEngineErrorCode;
class FNetworkManager;
class FEngineRenderingManager;
class FEngineTickingManager;
class ITickInterface;

class FEngine
{
friend FEngineManager;
	
protected:
	FEngine();
	virtual ~FEngine();

private:
	/** Called before tick. Once. */
	void EngineInit(int Argc, char* Argv[]);

	/** Is inside of loop. Runs until Exit() is called. */
	void EngineTick();

	/** Called once a second. */
	void EnginePostSecondTick();

public:
	/** First init ever called. */
	virtual void PreInit();

	/** Init (SDL, GEngine stuff). */
	virtual void Init();

	/** Third init function. */
	virtual void PostInit();

	/** To be sub-project overriden. Called each frame */
	virtual void Tick();

	/** To be sub-project overriden. Called every second */
	virtual void PostSecondTick();

	/** MainLoop() runs until this return false. */
	_NODISCARD virtual bool CanContinueMainLoop() const;

	/** True if frame rate limit is enabled. */
	_NODISCARD virtual bool IsFrameRateLimited() const;

	/** Call to stop main loop. (Exit engine) */
	virtual void RequestExit();

	/** Call to force stopping app */
	virtual void ForceExit(const EEngineErrorCode OptionalErrorCode = EEngineErrorCode::ForceExit);

	/** Called when main loops stop but before destructor. */
	virtual void PreExit();

	/** Clean up memory. (Remember to call parent! Otherwise you will leak memory.) */
	virtual void Clean();

	/** @returns true if Init() has finished */
	_NODISCARD bool IsEngineInitialized() const;

	virtual void UpdateFrameTimeStart();
	virtual void UpdateFrameTimeEnd();

	_NODISCARD uint32_t GetFrameTime() const;
	_NODISCARD uint32_t GetFrameDelay() const;

	/* Use to set engine frame rate - ticks per second */
	virtual void SetFrameRate(const uint32_t NewFrameRate);

	_NODISCARD int GetFramesThisSecond() const;

	virtual double GetMaxDeltaTime() const;
	virtual void SetDeltaTime(const double &InDeltaTime);

	_NODISCARD float GetDeltaTime() const;
	_NODISCARD double GetDeltaTimeDouble() const;

	void UpdateFrameTime();

	FRenderThread* GetRenderThread() const;

	/** @Returns engine render class (used for managing windows) */
	_NODISCARD FEngineRender* GetEngineRender() const;

	/** Use this if you changed to your own. Will return casted. */
	template<typename TRenderClass>
	TRenderClass* GetEngineRender() const
	{
		return static_cast<TRenderClass>(GetEngineRender());
	}
	_NODISCARD const std::string& GetLaunchFullPath() const;
	_NODISCARD const std::string& GetLaunchRelativePath() const;

	/** Call to add function to execute on next tick, FFunctorBase will be cleaned after executing. */
	void AddLambdaToCallOnStartOfNextTick(const FFunctorLambda<void>& Function);

	FDelegate<>& GetFunctionsToCallOnStartOfNextTick();

	_NODISCARD FEventHandler* GetEventHandler() const;

	/** Use this if you changed to your own. Will return casted. */
	template<typename TEngineRenderClass>
	TEngineRenderClass* GetEventHandler() const
	{
		return static_cast<TEngineRenderClass>(GetEngineRender());
	}

	_NODISCARD FAssetsManager* GetAssetsManager() const;

	/** Use this if you changed to your own. Will return casted. */
	template<typename TAssetsManager>
	TAssetsManager* GetAssetsManager() const
	{
		return static_cast<TAssetsManager>(GetAssetsManager());
	}

	_NODISCARD FEngineTickingManager* GetEngineTickingManager() const;
	_NODISCARD FEngineRenderingManager* GetEngineRenderingManager() const;
	_NODISCARD FThreadsManager* GetThreadsManager() const;
#if ENGINE_NETWORK_LIB_ENABLED
	_NODISCARD FNetworkManager* GetNetworkManager() const;
#endif

protected:
	void UpdateFrameRateCounter();

	_NODISCARD virtual FEngineRender* CreateEngineRenderer() const;
	_NODISCARD virtual FEventHandler* CreateEventHandler() const;
	_NODISCARD virtual FAssetsManager* CreateAssetsManager() const;
	_NODISCARD virtual FEngineTickingManager* CreateEngineTickingManager() const;
	_NODISCARD virtual FEngineRenderingManager* CreateEngineRenderingManager() const;
	_NODISCARD virtual FThreadsManager* CreateThreadsManager() const;

#if ENGINE_TESTS_ALLOW_ANY
	_NODISCARD virtual class FTestManager* CreateTestManager() const;
#endif

	static bool GetDisplaySettings(int DisplayIndex, SDL_DisplayMode& InDisplayMode);
	static bool GetPrimaryDisplaySettings(SDL_DisplayMode& InDisplayMode);

	FEngineLaunchParameter CreateEngineLaunchParameter(const std::string& InParameter) const;
	FEngineLaunchParameter GetLaunchParameter(const std::string& InName) const;

protected:
	bool bFrameRateLimited;
	bool bIsEngineInitialized;

	// Framerate per second / ticks per second
	uint32_t FrameRate;
	// Cached per frame delay for example 60 FPS will be 60 / 1000 = 16
	uint32_t FrameDelay;

	// Time ms of frame start
	uint32_t FrameStart;
	// Time ms of frame end
	uint32_t FrameTime;

	uint64_t CounterLastFrame;
	uint64_t CounterCurrentFrame;

	float DeltaTimeFloat;
	double DeltaTimeDouble;

	/** First param of main. Absolute location C:/Programs/ThisProgram/Program.exe */
	std::string LaunchFullPath;
	/** Location of exe without exe. Like C:/Programs/ThisProgram */
	std::string LaunchRelativePath;

	// Parameters
	CArray<FEngineLaunchParameter> LaunchParameters;

	/** Class for managing windows,  */
	FEngineRender* EngineRender;

	SDL_Event SdlEvent;
	FEventHandler* EventHandler;
	FAssetsManager* AssetsManager;

	FEngineTickingManager* EngineTickingManager;
	FEngineRenderingManager* EngineRenderingManager;
	FThreadsManager* ThreadsManager;

	FRenderThread* RenderThread;
	FThreadData* RenderThreadData;

	FDelegate<> FunctionsToCallOnStartOfNextTick;
	FDelegate<void, float> TickingObjectsDelegate;

#if ENGINE_NETWORK_LIB_ENABLED
	FNetworkManager* NetworkManager;
#endif

#if ENGINE_TESTS_ALLOW_ANY
	class FTestManager* TestManager;
#endif

private:
	bool bContinueMainLoop;
	int TicksThisSecond;
	size_t Second;

};
