// See https://github.com/Przemek2122/GameEngine

#pragma once

#include "CoreMinimal.h"
#include "Map.h"

/**
 * Base class for map with default map rendering
 */
class FEngineMap : public FMap
{
public:
	FEngineMap(FMapAsset* InMapAsset, FMapManager* InMapManager);
	~FEngineMap() override = default;

	/** Begin FMap */
	void Render() override;
	/** End FMap */

};
