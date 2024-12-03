// Created by Przemys³aw Wiewióra 2020-2024

#include "CoreEngine.h"
#include "Core/Engine/Logic/GameModeManager.h"
#include "Engine/Logic/GameModeBase.h"

FGameModeManager::FGameModeManager(FWindowAdvanced* InWindowAdvanced)
	: CurrentGameMode(nullptr)
	, OwnerWindowAdvanced(InWindowAdvanced)
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

FWindowAdvanced* FGameModeManager::GetOwnerWindowAdvanced() const
{
	return OwnerWindowAdvanced;
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
