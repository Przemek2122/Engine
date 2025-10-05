// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

class FGameModeManager;

// @TODO: Window advanced should be entirely removed.
// All of its functionality should be replaced by subsystems added to window

/**
 * Advanced window
 */
class ENGINE_API FWindowAdvanced : public FWindow
{
public:
	FWindowAdvanced(const std::string& InTitle, const FVector2D<int> InLocation, const FVector2D<int> InSize, Uint32 InWindowFlags = WINDOW_DEFAULT_FLAGS);

	FGameModeManager* GetGameModeManager() const { return GameModeManager; }

protected:
	FGameModeManager* GameModeManager;

};