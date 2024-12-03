// See https://github.com/Przemek2122/GameEngine

#pragma once

#include "CoreMinimal.h"

class FMapEditor;
class FMap;
class FMapAsset;

/**
 * Manager for maps.
 * Responsible for loading maps and managing them.
 *
 * How to use?
 * 1. CacheAvailableMaps
 * 2. Find map you would like to load. (by name or iterative, show on ui) string with name is required.
 * 3. Use LoadMap
 * 4. Use SetActiveGameMap or SetActiveEditorMap
 * 5. OPTIONAL - Map can be deactivated using DeactivateCurrentGameMap or DeactivateCurrentEditorMap
 */
class FMapManager
{
public:
	FMapManager(FWindow* InWindow);
	virtual ~FMapManager();

	virtual void TickMap(const float DeltaTime);
	virtual void DrawMap();

	/** Iterate over all maps to find this with given name. Inefficient. */
	static FMapAsset* GetMapByName(const std::string& Name);

	/** Sets map active in game. */
	void SetActiveGameMap(FMapAsset* MapAsset);

	/** Deactivates map */
	void DeactivateCurrentGameMap();

	/** Open editor for map */
	void SetActiveEditorMap(FMapAsset* MapAsset);

	/** Deactivates map - editor */
	void DeactivateCurrentEditorMap();

	/** Collect maps from their directory */
	virtual void CacheAvailableMaps();

	/** Get list of maps from directory. Make sure to call CacheAvailableMaps first. */
	virtual CArray<std::string> GetAvailableMaps() const;

	/** Load map asset */
	virtual FMapAsset* LoadMap(const std::string& Name);

	/** UnLoad map asset (destroy it's data) */
	virtual void UnLoadMap(const std::string& Name);

	/** UnLoad map asset (destroy it's data) */
	virtual void UnLoadMap(FMapAsset* MapAsset);

	/** Unload all maps which has been loaded. */
	virtual void UnloadAllMaps();

	/** Moves current map by given vector */
	void MoveMap(const FVector2D<int>& InMapRenderOffset) const;

	/** @returns Owner window. */
	FWindow* GetOwnerWindow() const { return OwnerWindow; };

	/** @returns true if MapAsset is currently used inside of current map */
	bool IsMapAssetCurrentlyUsed(const FMapAsset* MapAsset) const;

	FMap* GetCurrentMap() const;

	FMapEditor* GetMapEditor() const;

protected:
	/** Current map, has refrence to map asset and renders map. */
	FMap* CurrentMap;

	/** MapManager. Only active and valid when running map editor. */
	FMapEditor* MapEditor;

	/** List of available maps from maps directory. @See CacheAvailableMaps for more info */
	CArray<std::string> AvailableMaps;

	/** Available map assets. */
	CArray<FMapAsset*> MapAssets;

	/** Owner window. Required for MapAsset and Map classes */
	FWindow* OwnerWindow;

};
