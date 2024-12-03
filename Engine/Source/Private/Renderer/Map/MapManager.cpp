// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"
#include "Assets/Assets/MapAsset.h"
#include "Renderer/Map/Map.h"
#include "Renderer/Map/MapEditor.h"
#include "Renderer/Map/Mapmanager.h"

FMapManager::FMapManager(FWindow* InWindow)
	: CurrentMap(nullptr)
	, MapEditor(nullptr)
	, MapAssets()
	, OwnerWindow(InWindow)
{
}

FMapManager::~FMapManager()
{
	DeactivateCurrentEditorMap();

	DeactivateCurrentGameMap();
}

void FMapManager::TickMap(const float DeltaTime)
{
	if (CurrentMap != nullptr)
	{
		CurrentMap->Tick(DeltaTime);
	}
}

void FMapManager::DrawMap()
{
	if (CurrentMap != nullptr)
	{
		CurrentMap->Render();
	}
}

FMapAsset* FMapManager::GetMapByName(const std::string& Name)
{
	FAssetsManager* AssetsManager = GEngine->GetAssetsManager();
	if (AssetsManager != nullptr)
	{
		FMapAsset* MapAsset = AssetsManager->GetAsset<FMapAsset>(Name);
		if (MapAsset != nullptr)
		{
			if (MapAsset->IsLoaded())
			{
				return MapAsset;
			}
			else
			{
				LOG_WARN("Map is not loaded. Load it first.");
			}
		}
	}

	return nullptr;
}

void FMapManager::SetActiveGameMap(FMapAsset* MapAsset)
{
	if (MapAsset != nullptr)
	{
		if (MapAsset->IsLoaded())
		{
			if (CurrentMap != nullptr)
			{
				DeactivateCurrentGameMap();
			}

			CurrentMap = new FMap(MapAsset, this);
			CurrentMap->Initialize();
		}
		else
		{
			LOG_ERROR("Map is not loaded. Load it first.");
		}
	}
	else
	{
		LOG_ERROR("Map is nullptr.");
	}
}

void FMapManager::DeactivateCurrentGameMap()
{
	if (CurrentMap != nullptr)
	{
		CurrentMap->DeInitialize();

		delete CurrentMap;

		CurrentMap = nullptr;
	}
}

void FMapManager::SetActiveEditorMap(FMapAsset* MapAsset)
{
	if (MapAsset != nullptr)
	{
		if (MapAsset->IsLoaded())
		{
			if (CurrentMap != nullptr)
			{
				DeactivateCurrentEditorMap();
			}

			CurrentMap = new FMap(MapAsset, this);
			CurrentMap->Initialize();

			MapEditor = new FMapEditor(CurrentMap);
			MapEditor->Initialize();
		}
		else
		{
			LOG_ERROR("Map is not loaded. Load it first.");
		}
	}
	else
	{
		LOG_ERROR("Map is nullptr.");
	}
}

void FMapManager::DeactivateCurrentEditorMap()
{
	if (MapEditor != nullptr)
	{
		MapEditor->DeInitialize();

		delete MapEditor;

		MapEditor = nullptr;

		DeactivateCurrentGameMap();
	}
}

void FMapManager::CacheAvailableMaps()
{
	FAssetsManager* AssetsManager = GEngine->GetAssetsManager();
	if (AssetsManager != nullptr)
	{
		AvailableMaps.Clear();

		CArray<std::string> DirectoriesFound = AssetsManager->GetFilesFromDirectory(AssetsManager->GetMapsPathRelative());

		const char Slash = AssetsManager->GetPlatformSlash();

		const std::string PathPrefix = AssetsManager->GetMapsPathRelative() + Slash;

		for (std::string& Directory : DirectoriesFound)
		{
			std::string Path = PathPrefix + Directory;

			AssetsManager->AddAsset<FMapAsset>(Directory, Path);

			AvailableMaps.Push(Directory);
		}
	}
}

CArray<std::string> FMapManager::GetAvailableMaps() const
{
#if _DEBUG
	if (AvailableMaps.IsEmpty())
	{
		// Maps are empty,
		// either you forgot to call FMapManager::CacheAvailableMaps
		// or
		// maps are really empty
		ENSURE_VALID(false);

		LOG_ERROR("No maps found in FMapManager::GetAvailableMaps - Maps or empty or you did not call FMapManager::CacheAvailableMaps");
	}
#endif

	return AvailableMaps;
}

FMapAsset* FMapManager::LoadMap(const std::string& Name)
{
	FAssetsManager* AssetsManager = GEngine->GetAssetsManager();
	if (AssetsManager != nullptr)
	{
		FMapAsset* MapAsset = AssetsManager->GetAsset<FMapAsset>(Name);
		if (MapAsset != nullptr)
		{
			if (!MapAsset->IsLoaded())
			{
				MapAsset->SetMapManager(this);

				MapAsset->LoadMap();
			}

			return MapAsset;
		}
		else
		{
			LOG_ERROR("Map not found: " + Name);
		}
	}

	return nullptr;
}

void FMapManager::UnLoadMap(const std::string& Name)
{
	if (!Name.empty())
	{
		FMapAsset* MapAsset = GetMapByName(Name);
		if (MapAsset != nullptr)
		{
			UnLoadMap(MapAsset);
		}
		else
		{
			LOG_ERROR("Map not found: " + Name);
		}
	}
	else
	{
		LOG_ERROR("Map name is empty.");
	}
}

void FMapManager::UnLoadMap(FMapAsset* MapAsset)
{
	if (MapAsset != nullptr && MapAsset->IsLoaded())
	{
		if (IsMapAssetCurrentlyUsed(MapAsset))
		{
			LOG_ERROR("Tried to unload current map. This is not allowed. Use DeactivateCurrentGameMap first instead.");
		}
		else
		{
			MapAsset->ClearMapData();
		}
	}
}

void FMapManager::UnloadAllMaps()
{
	for (FMapAsset* MapAsset : MapAssets)
	{
		UnLoadMap(MapAsset);
	}
}

void FMapManager::MoveMap(const FVector2D<int>& InMapRenderOffset) const
{
	if (CurrentMap != nullptr)
	{
		CurrentMap->AddMapRenderOffset(InMapRenderOffset);
	}
}

bool FMapManager::IsMapAssetCurrentlyUsed(const FMapAsset* MapAsset) const
{
	return (CurrentMap != nullptr && CurrentMap->GetMapAsset() == MapAsset);
}

FMap* FMapManager::GetCurrentMap() const
{
	return CurrentMap;
}

FMapEditor* FMapManager::GetMapEditor() const
{
	return MapEditor;
}
