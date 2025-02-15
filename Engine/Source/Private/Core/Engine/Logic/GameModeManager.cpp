// Created by Przemys³aw Wiewióra 2020-2024

#include "CoreEngine.h"
#include "Core/Engine/Logic/GameModeManager.h"
#include "Engine/Logic/GameModeBase.h"

FGameModeManager::FGameModeManager(FWindow* InWindow)
	: CurrentGameMode(nullptr)
	, OwnerWindow(InWindow)
{
}

FGameModeManager::~FGameModeManager()
{
	CleanPreviousGameMode();
}

FGameModeBase* FGameModeManager::GetCurrentGameMode() const
{
	return CurrentGameMode;
}

FWindow* FGameModeManager::GetOwnerWindow() const
{
	return OwnerWindow;
}

void FGameModeManager::CleanPreviousGameMode()
{
	if (CurrentGameMode)
	{
		CurrentGameMode->Finish();
		CurrentGameMode->DeInitialize();

		delete CurrentGameMode;

		CurrentGameMode = nullptr;
	}
}
