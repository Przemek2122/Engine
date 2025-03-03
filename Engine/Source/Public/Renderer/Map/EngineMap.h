// See https://github.com/Przemek2122/GameEngine

#pragma once

#include "CoreMinimal.h"
#include "Map.h"

class FEngineMap : public FMap
{
public:
	FEngineMap(FMapAsset* InMapAsset, FMapManager* InMapManager);
	~FEngineMap() override = default;


};
