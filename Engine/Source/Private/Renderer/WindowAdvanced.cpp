// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Renderer/WindowAdvanced.h"

#include "Engine/Logic/GameModeManager.h"

FWindowAdvanced::FWindowAdvanced(const std::string& InTitle, const FVector2D<int> InLocation, const FVector2D<int> InSize, Uint32 InWindowFlags)
	: FWindow(InTitle, InLocation, InSize, InWindowFlags)
{
	GameModeManager = CreateSubSystem<FGameModeManager>(this);
}