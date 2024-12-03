#pragma once

#include "CoreMinimal.h"

class FMap;

/**
 * Class for editing the map
 * It should have references to the map
 * Map should not have references to it
 */
class FMapEditor
{
public:
	FMapEditor(FMap* InMap);

	void Initialize();
	void DeInitialize();

	/** Called from UI - when user selects tile index on UI which he wants to use */
	void SelectTileIndex(int Index);

	/** Called from UI - When user clicks certain location */
	void OnMapClicked(const FVector2D<int>& Location) const;

	/** Called from UI - When user clicks certain location */
	void SaveMap() const;

protected:
	/** Edited map */
	FMap* Map;

	/** Index of edited tile. */
	int SelectedTileIndex;

};
