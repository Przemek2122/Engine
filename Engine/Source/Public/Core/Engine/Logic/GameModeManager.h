// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "Renderer/WindowAdvanced.h"

class FGameModeBase;

/**
 * Manages game modes used in FGameModeManager.
 */
class FGameModeManager : public ISubSystemInstanceInterface
{
public:
	FGameModeManager(FWindowAdvanced* InWindowAdvanced);
	~FGameModeManager() override;

	/** Create object of type FGameModeBase. @Note previous game mode gets deleted */
	template<typename TGameModeClass>
	TGameModeClass* CreateGameMode(const bool bAutoStartGameMode)
	{
		CleanPreviousGameMode();

		TGameModeClass* GameMode = new TGameModeClass(this);

		CurrentGameMode = GameMode;

		GameMode->Initialize();

		if (bAutoStartGameMode)
		{
			GameMode->Begin();
		}

		return GetCurrentGameMode<TGameModeClass>();
	}

	/** @returns CurrentGameMode as base class */
	FGameModeBase* GetCurrentGameMode() const;

	/** @returns CurrentGameMode as class given in template parameter */
	template<typename TGameModeClass>
	TGameModeClass* GetCurrentGameMode()
	{
		return dynamic_cast<TGameModeClass*>(CurrentGameMode);
	}

	FWindowAdvanced* GetOwnerWindowAdvanced() const;

protected:
	void CleanPreviousGameMode();

private:
	FGameModeBase* CurrentGameMode;

	FWindowAdvanced* OwnerWindowAdvanced;

};
