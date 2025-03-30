// Created by Przemys³aw Wiewióra 2020-2024 https://github.com/Przemek2122/GameEngine

#pragma once

#include "CoreMinimal.h"
#include "Includes/EngineErrorCodes.h"
#include "Includes/EngineLaunchParameterCollection.h"

class IEngineSubsystemInterface;
class FIniObject;
enum class EEngineErrorCode;
class FNetworkManager;
class FEngineRenderingManager;
class FEngineTickingManager;
class ITickInterface;

class ENGINE_API FEngine
{
friend FEngineManager;
	
public:
	FEngine();
	virtual ~FEngine();

private:
	/** Called before tick. Once. */
	void EngineInit(int Argc, char* Argv[]);

	/** Called once after EngineInit on other thread */
	void EngineInitAsync();

	/** Is inside of loop. Runs until Exit() is called. */
	void EngineTick();

	/** Called once a second. */
	void EnginePostSecondTick();

public:
	/** First init ever called. */
	virtual void PreInit();

	/** Init (SDL, FGlobalDefines::GEngine stuff). */
	virtual void Init();

	/** Third init function. */
	virtual void PostInit();

	/**
	 * Called once after EngineInit on other thread
	 * Not everything can be async, so when you are not sure when to use it, use Init() instead.
	 */
	virtual void InitAsync();

	/** To be sub-project overriden. Called each frame */
	virtual void Tick();

	/** To be sub-project overriden. Called every second */
	virtual void PostSecondTick();

	/** MainLoop() runs until this return false. */
	NO_DISCARD virtual bool CanContinueMainLoop() const;

	/** True if frame rate limit is enabled. */
	NO_DISCARD virtual bool IsFrameRateLimited() const;

	/** Call to stop main loop. (Exit engine) */
	virtual void RequestExit();

	/** Call to force stopping app */
	virtual void ForceExit(const EEngineErrorCode OptionalErrorCode = EEngineErrorCode::ForceExit);

	/** Called when main loops stop but before destructor. */
	virtual void PreExit();

	/** Clean up memory. (Remember to call parent! Otherwise you will leak memory.) */
	virtual void Clean();

	/** @returns true if Init() has finished */
	NO_DISCARD bool IsEngineInitialized() const;

	virtual void UpdateFrameTimeStart();
	virtual void UpdateFrameTimeEnd();

	NO_DISCARD uint32_t GetFrameTime() const;
	NO_DISCARD uint32_t GetFrameDelay() const;

	/* Use to set engine frame rate - ticks per second */
	virtual void SetFrameRate(const uint32_t NewFrameRate);

	NO_DISCARD int GetFramesThisSecond() const;

	virtual double GetMaxDeltaTime() const;
	virtual void SetDeltaTime(const double &InDeltaTime);

	NO_DISCARD float GetDeltaTime() const;
	NO_DISCARD double GetDeltaTimeDouble() const;

	void UpdateFrameTime();

	FRenderThread* GetRenderThread() const;

	/** @Returns engine render class (used for managing windows) */
	NO_DISCARD FEngineRender* GetEngineRender() const;

	/** Use this if you changed to your own. Will return casted. */
	template<typename TRenderClass>
	TRenderClass* GetEngineRender() const
	{
		return static_cast<TRenderClass>(GetEngineRender());
	}
	NO_DISCARD const std::string& GetLaunchFullPath() const;
	NO_DISCARD const std::string& GetLaunchRelativePath() const;

	/** Call to add function to execute on next tick, FFunctorBase will be cleaned after executing. */
	void AddLambdaToCallOnStartOfNextTick(const FFunctorLambda<void>& Function);

	void DeInitializeEngineSubsystems();

	void TickEngineSubsystems();

	/** Create new Engine subsystem from given class */
	template<class TEngineSubsystemInterfaceClass>
	std::shared_ptr<TEngineSubsystemInterfaceClass> CreateEngineSubsystem()
	{
		std::shared_ptr<TEngineSubsystemInterfaceClass> Out = std::make_shared<TEngineSubsystemInterfaceClass>();

		Out->Initialize();

		ManagedEngineSubsystems.Push(Out);

		return Out;
	}

	/** Destroy Engine subsystem from by pointer */
	void DestroyEngineSubsystem(const std::shared_ptr<IEngineSubsystemInterface>& InEngineSubsystem);

	/** Get engine subsystem by class */
	template<class TEngineSubsystemInterfaceClass>
	std::shared_ptr<TEngineSubsystemInterfaceClass> GetSubsystemByClass()
	{
		std::shared_ptr<TEngineSubsystemInterfaceClass> Out;

		for (const std::shared_ptr<IEngineSubsystemInterface>& EngineSubsystemPtr : ManagedEngineSubsystems)
		{
			if (dynamic_cast<TEngineSubsystemInterfaceClass*>(EngineSubsystemPtr.get()) != nullptr)
			{
				Out = std::dynamic_pointer_cast<TEngineSubsystemInterfaceClass>(EngineSubsystemPtr);
			}
		}

		return Out;
	}

	FDelegate<>& GetFunctionsToCallOnStartOfNextTick();

	NO_DISCARD FEventHandler* GetEventHandler() const;

	/** Use this if you changed to your own. Will return casted. */
	template<typename TEngineRenderClass>
	TEngineRenderClass* GetEventHandler() const
	{
		return static_cast<TEngineRenderClass>(GetEngineRender());
	}

	NO_DISCARD FAssetsManager* GetAssetsManager() const;

	/** Use this if you changed to your own. Will return casted. */
	template<typename TAssetsManager>
	TAssetsManager* GetAssetsManager() const
	{
		return static_cast<TAssetsManager>(GetAssetsManager());
	}

	NO_DISCARD FEngineTickingManager* GetEngineTickingManager() const;
	NO_DISCARD FEngineRenderingManager* GetEngineRenderingManager() const;
	NO_DISCARD FThreadsManager* GetThreadsManager() const;
#if ENGINE_NETWORK_LIB_ENABLED
	NO_DISCARD FNetworkManager* GetNetworkManager() const;
#endif

protected:
	void UpdateFrameRateCounter();

	NO_DISCARD virtual FEngineRender* CreateEngineRenderer() const;
	NO_DISCARD virtual FEventHandler* CreateEventHandler() const;
	NO_DISCARD virtual FAssetsManager* CreateAssetsManager() const;
	NO_DISCARD virtual FEngineTickingManager* CreateEngineTickingManager() const;
	NO_DISCARD virtual FEngineRenderingManager* CreateEngineRenderingManager() const;
	NO_DISCARD virtual FThreadsManager* CreateThreadsManager() const;

#if ENGINE_TESTS_ALLOW_ANY
	NO_DISCARD virtual class FTestManager* CreateTestManager() const;
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

	/** Array with managed subsystems */
	CArray<std::shared_ptr<IEngineSubsystemInterface>> ManagedEngineSubsystems;

	std::shared_ptr<FIniObject> ProjectIni;

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
