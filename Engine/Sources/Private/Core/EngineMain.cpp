// Created by https://www.linkedin.com/in/przemek2122/ 2020

#include "CoreEngine.h"
#include "EngineMain.h"

void FEngineManager::Start(const int InArgc, char* InArgv[])
{
	std::cout << "Application starting ..." << std::endl;

	FGlobalDefines::GEngine = EngineClass.Allocate();
		
	Init(InArgc, InArgv);

	MainLoop();

	Exit();
}

void FEngineManager::MainLoop()
{
	while (FGlobalDefines::GEngine->CanContinueMainLoop())
	{
		LoopIterate();

		// Delay if required.
		if (FGlobalDefines::GEngine->IsFrameRateLimited())
		{
			const auto EngineFrameTime = FGlobalDefines::GEngine->GetFrameTime();
			const auto EngineFrameDelay = FGlobalDefines::GEngine->GetFrameDelay();

			if (EngineFrameDelay > EngineFrameTime)
			{
				SDL_Delay(EngineFrameDelay - EngineFrameTime);
			}
		}

		FGlobalDefines::GEngine->CounterLastFrame = FGlobalDefines::GEngine->CounterCurrentFrame;
	}
}

void FEngineManager::Init(const int Argc, char* Argv[])
{
	FGlobalDefines::GEngine->PreInit();

	FGlobalDefines::GEngine->EngineInit(Argc, Argv);

	FGlobalDefines::GEngine->Init();

	FGlobalDefines::GEngine->PostInit();
}

void FEngineManager::LoopIterate()
{
	FGlobalDefines::GEngine->UpdateFrameTimeStart();

	// Calculate DeltaTime
	{
		FGlobalDefines::GEngine->CounterCurrentFrame = SDL_GetPerformanceCounter();
		auto DeltaTime = static_cast<double>(FGlobalDefines::GEngine->CounterCurrentFrame - FGlobalDefines::GEngine->CounterLastFrame) / static_cast<double>(SDL_GetPerformanceFrequency());
		FGlobalDefines::GEngine->SetDeltaTime(DeltaTime);
	}

	FGlobalDefines::GEngine->EngineTick();

	FGlobalDefines::GEngine->UpdateFrameTimeEnd();
}

void FEngineManager::Exit()
{
	FGlobalDefines::GEngine->PreExit();

	FGlobalDefines::GEngine->Clean();

	delete FGlobalDefines::GEngine;

	std::cout << "Engine end." << std::endl;
}

FEngine* FEngineManager::Get()
{
	return FGlobalDefines::GEngine;
}
