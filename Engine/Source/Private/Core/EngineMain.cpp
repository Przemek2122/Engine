// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "EngineMain.h"

void FEngineManager::Start(const int InArgc, char* InArgv[])
{
	std::cout << "Game engine initializing ..." << std::endl;

	GEngine = EngineClass.Allocate();
		
	Init(InArgc, InArgv);

	MainLoop();

	Exit();
}

void FEngineManager::MainLoop()
{
	while (GEngine->CanContinueMainLoop())
	{
		LoopIterate();

		// Delay if required.
		if (GEngine->IsFrameRateLimited())
		{
			const auto EngineFrameTime = GEngine->GetFrameTime();
			const auto EngineFrameDelay = GEngine->GetFrameDelay();

			if (EngineFrameDelay > EngineFrameTime)
			{
				SDL_Delay(EngineFrameDelay - EngineFrameTime);
			}
		}

		GEngine->CounterLastFrame = GEngine->CounterCurrentFrame;
	}
}

void FEngineManager::Init(const int Argc, char* Argv[])
{
	GEngine->PreInit();

	GEngine->EngineInit(Argc, Argv);

	GEngine->Init();

	GEngine->PostInit();
}

void FEngineManager::LoopIterate()
{
	GEngine->UpdateFrameTimeStart();

	// Calculate DeltaTime
	{
		GEngine->CounterCurrentFrame = SDL_GetPerformanceCounter();
		auto DeltaTime = static_cast<double>(GEngine->CounterCurrentFrame - GEngine->CounterLastFrame) / static_cast<double>(SDL_GetPerformanceFrequency());
		GEngine->SetDeltaTime(DeltaTime);
	}
		
	GEngine->EngineTick();

	GEngine->UpdateFrameTimeEnd();
}

void FEngineManager::Exit()
{
	GEngine->PreExit();

	GEngine->Clean();

	delete GEngine;

	std::cout << "Game engine end." << std::endl;
}

FEngine* FEngineManager::Get()
{
	return GEngine;
}
