// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "EngineMain.h"

FEngineManager::FEngineManager()
{
}

FEngineManager::~FEngineManager()
{
}

FEngine* FEngineManager::Get()
{
	return GEngine;
}

void FEngineManager::Init(const int Argc, char* Argv[])
{
	GEngine->PreInit();

	GEngine->EngineInit(Argc, Argv);

	GEngine->Init();

	GEngine->PostInit();
}

void FEngineManager::MainLoop()
{
	while (GEngine->CanContinueMainLoop())
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

void FEngineManager::Exit()
{
	GEngine->PreExit();

	GEngine->Clean();

	delete GEngine;

	std::cout << "Game engine end." << std::endl;
}
