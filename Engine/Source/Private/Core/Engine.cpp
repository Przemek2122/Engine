// Created by Przemys³aw Wiewióra 2020-2024 https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"
#include "Input/EventHandler.h"

#if ENGINE_TESTS_ALLOW_ANY
#include "Test/TestManager.h"

#include "Test/Samples/TestDelegate.h"
#include "Test/Samples/TestTypes.h"
#include "Test/Samples/TestClassType.h"
#endif

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/NetworkManager.h"
#endif

#include "Assets/Assets/FontAsset.h"
#include "Engine/EngineRenderingManager.h"
#include "Engine/EngineTickingManager.h"
#include "Includes/EngineLaunchParameterCollection.h"
#include "Misc/StringHelpers.h"
#include "Test/Samples/TestPerformance.h"
#include "Test/Samples/TestTimers.h"
#include "Threads/RenderThread.h"
#include "Threads/ThreadsManager.h"
#include "Threads/ThreadData.h"

FEngine::FEngine()
	: bFrameRateLimited(true)
	, bIsEngineInitialized(false)
	, FrameRate(0)
	, FrameDelay(0)
	, FrameStart(0)
	, FrameTime(0)
	, CounterLastFrame(0)
	, CounterCurrentFrame(SDL_GetPerformanceCounter())
	, DeltaTimeFloat(0)
	, DeltaTimeDouble(0)
	, EngineRender(nullptr)
	, SdlEvent()
	, EventHandler(nullptr)
	, AssetsManager(nullptr)
	, EngineTickingManager(nullptr)
	, EngineRenderingManager(nullptr)
	, ThreadsManager(nullptr)
	, RenderThreadData(nullptr)
#if ENGINE_TESTS_ALLOW_ANY
	, TestManager(nullptr)
#endif
	, bContinueMainLoop(true)
	, TicksThisSecond(0)
	, Second(0)
{
	FUtil::LogInit();
}

FEngine::~FEngine()
{
	LOG_INFO("GEngine finish (is being destroyed now).");

	TTF_Quit();
	SDL_Quit();
}

void FEngine::EngineInit(int Argc, char* Argv[])
{
#if defined(__DATE__) && defined(__TIME__)
	LOG_INFO("GEngine init start compiled: " << __DATE__ << " " << __TIME__);
#else
	LOG_INFO("GEngine init start.");
#endif

	// Read command line flags.
	while (Argc--)
	{
		if (Argc == 0)
		{
			LaunchFullPath = Argv[Argc];

			LOG_INFO("Found launch exe path [" << Argc << "] = " << Argv[Argc]);
		}
		else
		{
			LaunchParameters.Push(CreateEngineLaunchParameter(Argv[Argc]));
		}
	}

	// Print parameters
	for (FEngineLaunchParameter& LaunchParameter : LaunchParameters)
	{
		LOG_DEBUG("Param name: " << LaunchParameter.Name << ", value: " << LaunchParameter.Value);
	}

	// We always need correct directory.
	if (ENSURE_VALID(!LaunchFullPath.empty()))
	{
		LaunchRelativePath = LaunchFullPath.substr(0, LaunchFullPath.find_last_of('\\'));
	}

	// Initialize SDL
	const bool SdlInitialized = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	if (SdlInitialized)
	{
		LOG_INFO("SDL Subsystems Initialised!");
	}
	else
	{
		LOG_ERROR("SDL_INIT_EVERYTHING error: " << SDL_GetError());

		ForceExit(EEngineErrorCode::SDL_InitFail);
	}

	// Initialize SDL TTF 
	if (!TTF_Init())
	{
		LOG_ERROR("TTF_Init: " << SDL_GetError());

		ForceExit(EEngineErrorCode::TTF_InitFail);
	}

	// Initialize SDL - Load support for everything supported
	constexpr auto MixFlags = MIX_INIT_OGG | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_MID | MIX_INIT_OPUS;
	const int MixInitialized = Mix_Init(MixFlags);
	if (!MixInitialized)
	{
		LOG_ERROR("Mix_Init: " << SDL_GetError());

		ForceExit(EEngineErrorCode::Mix_InitFail);
	}

	SDL_AudioSpec AudioSpec;
	AudioSpec.freq = 44100;
	AudioSpec.format = MIX_DEFAULT_FORMAT;
	AudioSpec.channels = 2;
	if (!Mix_OpenAudio(0, &AudioSpec))
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", SDL_GetError());

		ForceExit(EEngineErrorCode::Mixer_OpenAudioFailed);
	}

	EngineRender = CreateEngineRenderer();
	EventHandler = CreateEventHandler();
	AssetsManager = CreateAssetsManager();
	EngineTickingManager = CreateEngineTickingManager();
	EngineRenderingManager = CreateEngineRenderingManager();
	ThreadsManager = CreateThreadsManager();

	EventHandler->InitializeInputFromConfig();

	ThreadsManager->Initialize();

	// Add render thread
	RenderThreadData = ThreadsManager->CreateThread<FRenderThread, FThreadData>("RenderThreadData");
	RenderThread = dynamic_cast<FRenderThread*>(RenderThreadData->GetThread());

#if ENGINE_NETWORK_LIB_ENABLED
	const FEngineLaunchParameter& IsEngineLaunchParameter = GetLaunchParameter(FEngineLaunchParameterCollection::IsServer);

	bool bIsServer = true;
	if (IsEngineLaunchParameter.IsValid())
	{
		bIsServer = IsEngineLaunchParameter.AsBool();
	}

	NetworkManager = new FNetworkManager(this, bIsServer);
	NetworkManager->Initialize();
#endif

#if ENGINE_TESTS_ALLOW_ANY
	TestManager = CreateTestManager();
#endif

#if ENGINE_TESTS_ALLOW_ANY && ENGINE_TESTS_RUN
	TestManager->SpawnTestCaseByClass<FTestTypes>();
	TestManager->SpawnTestCaseByClass<FTestDelegate>();
	TestManager->SpawnTestCaseByClass<FTestClassType>();
	TestManager->SpawnTestCaseByClass<FTimersLongRunTest>();
	TestManager->SpawnTestCaseByClass<FTestPerformance>();
#endif

	AssetsManager->AddAsset<FFontAsset>("OpenSans", R"(Assets\Fonts\OpenSans\OpenSans-Regular.ttf)");

	UpdateFrameTime();

	LOG_INFO("GEngine init End");

	bIsEngineInitialized = true;
}

void FEngine::EngineTick()
{
	UpdateFrameRateCounter();

	EventHandler->HandleEvents();

	if (EventHandler->QuitInputDetected())
	{
		GEngine->RequestExit();
	}

	// Tick functions for next tick
	if (FunctionsToCallOnStartOfNextTick.IsBound())
	{
		FunctionsToCallOnStartOfNextTick.Execute();

		FunctionsToCallOnStartOfNextTick.UnBindAll();
	}

	// Tick objects registered with TickInterface
	TickingObjectsDelegate.Execute(DeltaTimeFloat);

	Tick();

	EngineTickingManager->EngineTick(DeltaTimeFloat);

	EngineRender->Tick();

	ThreadsManager->TickThreadCallbacks();

	EngineRenderingManager->EngineRender();

	// Wait for Render thread.
	// We need to do this to avoid changing data when render is not finished
	while (!RenderThread->IsRenderingFrameFinished())
	{
		THREAD_WAIT_SHORT_TIME;
	}

	RenderThread->AllowRenderNextFrame();
}

void FEngine::EnginePostSecondTick()
{
	PostSecondTick();
}

void FEngine::PreInit()
{
}

void FEngine::Init()
{
	
}

void FEngine::PostInit()
{
}

void FEngine::Tick()
{

}

void FEngine::PostSecondTick()
{
}

bool FEngine::CanContinueMainLoop() const
{
	return bContinueMainLoop;
}

bool FEngine::IsFrameRateLimited() const
{
	return bFrameRateLimited;
}

void FEngine::RequestExit()
{
	bContinueMainLoop = false;
}

void FEngine::ForceExit(const EEngineErrorCode OptionalErrorCode)
{
	Clean();

	LOG_WARN("Waiting for log to be printed.");

	while (!FUtil::MessagesQueue.IsEmpty())
	{
		THREAD_WAIT_SHORT_TIME;
	}

	exit(static_cast<int>(OptionalErrorCode));
}

void FEngine::PreExit()
{
}

void FEngine::Clean()
{
	if (RenderThread != nullptr)
	{
		RenderThread->StopThread();
	}
	else
	{
		LOG_ERROR("Render thread does not exist before cleaning");
	}

	delete EngineRender;
	delete EventHandler;
	delete AssetsManager;
	delete EngineTickingManager;
	delete EngineRenderingManager;
	delete ThreadsManager;

#if ENGINE_NETWORK_LIB_ENABLED
	NetworkManager->DeInitialize();
	delete NetworkManager;
#endif

#if ENGINE_TESTS_ALLOW_ANY
	delete TestManager;
#endif
}

bool FEngine::IsEngineInitialized() const
{
	return bIsEngineInitialized;
}

void FEngine::UpdateFrameRateCounter()
{
	const auto SystemTime = FUtil::GetSeconds();

	if (Second == SystemTime)
	{
		TicksThisSecond++;
	}
	else
	{
		EnginePostSecondTick();

		TicksThisSecond = 0;
		Second = SystemTime;
	}
}

void FEngine::UpdateFrameTimeStart()
{
	CounterLastFrame = CounterCurrentFrame;

	CounterCurrentFrame = SDL_GetPerformanceCounter();

	FrameStart = SDL_GetTicks();
}

void FEngine::UpdateFrameTimeEnd()
{
	FrameTime = SDL_GetTicks() - FrameStart;
}

uint32_t FEngine::GetFrameTime() const
{
	return FrameTime;
}

uint32_t FEngine::GetFrameDelay() const
{
	return FrameDelay;
}

void FEngine::SetFrameRate(const uint32_t NewFrameRate)
{
	FrameRate = NewFrameRate;
	FrameDelay = 1000 / FrameRate;
}

int FEngine::GetFramesThisSecond() const
{
	return TicksThisSecond;
}

double FEngine::GetMaxDeltaTime() const
{
	return 0.3;
}

void FEngine::SetDeltaTime(const double &InDeltaTime)
{
	const auto LocalDeltaTimeMax = GetMaxDeltaTime();
	
	if (InDeltaTime < LocalDeltaTimeMax)
	{
		DeltaTimeDouble = InDeltaTime;
		DeltaTimeFloat = static_cast<float>(InDeltaTime);	
	}
	else
	{
		DeltaTimeDouble = LocalDeltaTimeMax;
		DeltaTimeFloat = static_cast<float>(LocalDeltaTimeMax);
	}
}

float FEngine::GetDeltaTime() const
{
	return DeltaTimeFloat;
}

double FEngine::GetDeltaTimeDouble() const
{
	return DeltaTimeDouble;
}

void FEngine::UpdateFrameTime()
{
	// Default used in case where there is issue with screen settings
	const int DefaultTargetFrameRate = 60;

	int TargetFrameRate;
	SDL_DisplayMode DisplayMode;
	if (GetPrimaryDisplaySettings(DisplayMode))
	{
		// Take refresh rate from display
		TargetFrameRate = DisplayMode.refresh_rate;
	}
	else
	{
		TargetFrameRate = DefaultTargetFrameRate;

		LOG_ERROR("Unable to get target framerate. Default of " << DefaultTargetFrameRate << "Will be used instead.");
	}

	SetFrameRate(TargetFrameRate);
}

FRenderThread* FEngine::GetRenderThread() const
{
	return RenderThread;
}

FEngineRender* FEngine::CreateEngineRenderer() const
{
	return new FEngineRender();
}

FEventHandler* FEngine::CreateEventHandler() const
{
	return new FEventHandler(SdlEvent);
}

FAssetsManager* FEngine::CreateAssetsManager() const
{
	return new FAssetsManager;
}

FEngineTickingManager* FEngine::CreateEngineTickingManager() const
{
	return new FEngineTickingManager;
}

FEngineRenderingManager* FEngine::CreateEngineRenderingManager() const
{
	return new FEngineRenderingManager;
}

FThreadsManager* FEngine::CreateThreadsManager() const
{
	return new FThreadsManager();
}

const std::string& FEngine::GetLaunchFullPath() const
{
	return LaunchFullPath;
}

const std::string& FEngine::GetLaunchRelativePath() const
{
	return LaunchRelativePath;
}

void FEngine::AddLambdaToCallOnStartOfNextTick(const FFunctorLambda<void>& Function)
{
	FunctionsToCallOnStartOfNextTick.BindLambda(Function);
}

FDelegate<void>& FEngine::GetFunctionsToCallOnStartOfNextTick()
{
	return FunctionsToCallOnStartOfNextTick;
}

FEventHandler* FEngine::GetEventHandler() const
{
#if _DEBUG
		ENSURE_VALID(EventHandler != nullptr);
#endif
		
	return EventHandler;
}

FAssetsManager* FEngine::GetAssetsManager() const
{
	return AssetsManager;
}

FEngineTickingManager* FEngine::GetEngineTickingManager() const
{
	return EngineTickingManager;
}

FEngineRenderingManager* FEngine::GetEngineRenderingManager() const
{
	return EngineRenderingManager;
}

FThreadsManager* FEngine::GetThreadsManager() const
{
	return ThreadsManager;
}

#if ENGINE_NETWORK_LIB_ENABLED
FNetworkManager* FEngine::GetNetworkManager() const
{
	return NetworkManager;
}
#endif

#if ENGINE_TESTS_ALLOW_ANY
FTestManager* FEngine::CreateTestManager() const
{
	return new FTestManager;
}
#endif

FEngineRender* FEngine::GetEngineRender() const
{
#if _DEBUG
		ENSURE_VALID(EngineRender != nullptr);
#endif
	
	return EngineRender;
}

bool FEngine::GetDisplaySettings(const int DisplayIndex, SDL_DisplayMode& InDisplayMode)
{
	bool bIsSuccessful = false;

	int* NumberOfDisplays = nullptr;
	SDL_DisplayID* DisplayIDArray = SDL_GetDisplays(NumberOfDisplays);

	if (NumberOfDisplays != nullptr && *NumberOfDisplays > DisplayIndex)
	{
		SDL_DisplayID DisplayId = DisplayIDArray[DisplayIndex];
		const SDL_DisplayMode* DisplayMode = SDL_GetCurrentDisplayMode(DisplayId);
		if (DisplayMode != nullptr)
		{
			InDisplayMode = *DisplayMode;

			bIsSuccessful = true;
		}
	}
	else
	{
		LOG_WARN("Error getting display settings: '" << SDL_GetError() << "'.");
	}

	return bIsSuccessful;
}

bool FEngine::GetPrimaryDisplaySettings(SDL_DisplayMode& InDisplayMode)
{
	bool bIsSuccessful = false;

	SDL_DisplayID DisplayID = SDL_GetPrimaryDisplay();
	if (DisplayID != 0)
	{
		const SDL_DisplayMode* DisplayMode = SDL_GetCurrentDisplayMode(DisplayID);
		if (DisplayMode != nullptr)
		{
			InDisplayMode = *DisplayMode;

			bIsSuccessful = true;
		}
	}
	else
	{
		LOG_WARN("Error getting display settings: '" << SDL_GetError() << "'.");
	}

	return bIsSuccessful;
}

FEngineLaunchParameter FEngine::CreateEngineLaunchParameter(const std::string& InParameter) const
{
	FEngineLaunchParameter EngineLaunchParameter;

	bool bMinusFound = false;
	bool bEqualSignFound = false;

	for (int32 i = 0; i < 1024 && InParameter.length() > i; i++)
	{
		if (bEqualSignFound)
		{
			EngineLaunchParameter.Value += InParameter[i];
		}
		else if (bMinusFound)
		{
			if (InParameter[i] == '=')
			{
				bEqualSignFound = true;
			}
			else
			{
				EngineLaunchParameter.Name += InParameter[i];
			}
		}
		else if (InParameter[i] == '-')
		{
			bMinusFound = true;
		}
	}

	return EngineLaunchParameter;
}

FEngineLaunchParameter FEngine::GetLaunchParameter(const std::string& InName) const
{
	FEngineLaunchParameter EngineLaunchParameter;

	// @TODO Would be way better using a map instead of an array
	for (const FEngineLaunchParameter& LaunchParameter : LaunchParameters)
	{
		if (LaunchParameter.Name == InName)
		{
			EngineLaunchParameter = LaunchParameter;

			break;
		}
	}

	return EngineLaunchParameter;
}
