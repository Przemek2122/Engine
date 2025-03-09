// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"
#include "Renderer/Map/Map.h"
#include "ECS/Entities/CameraManager.h"
#include "Misc/Math.h"
#include "Renderer/Map/MapManager.h"

FMap::FMap(FMapAsset* InMapAsset, FMapManager* InMapManager)
	: MapAsset(InMapAsset)
	, MapManager(InMapManager)
	, CameraManagerEntity(nullptr)
	, bIsActive(false)
	, EntityManager(nullptr)
{
}

void FMap::Initialize()
{
	EntityManager = CreateEntityManager();

	ReadAsset();

	CameraManagerEntity = EntityManager->CreateEntity<ECameraManager>();

	UpdateMapOffsetOnRenderer();

	MapManager->GetOwnerWindow()->OnWindowSizeChangedDelegate.BindObject(this, &FMap::OnWindowSizeChanged);
}

void FMap::DeInitialize()
{
	MapManager->GetOwnerWindow()->OnWindowSizeChangedDelegate.UnBindObject(this, &FMap::OnWindowSizeChanged);

	ClearData();

	if (EntityManager != nullptr)
	{
		EntityManager->DestroyEntity(CameraManagerEntity);

		delete EntityManager;
	}
}

void FMap::Tick(float DeltaTime)
{
	if (bIsActive)
	{
		EntityManager->Tick(DeltaTime);

		TickSubSystems();
	}
}

void FMap::Render()
{
	if (bIsActive)
	{
		EntityManager->Render();

		RenderSubSystems();
	}
}

void FMap::Load()
{
	MapAsset->LoadMap();

	bIsActive = MapAsset->IsLoaded();
}

void FMap::ClearData()
{
	MapData.Clear();

	MapAsset->ClearMapData();

	bIsActive = true;
}

void FMap::Save()
{
	// Write data from this class to FMapAsset
	WriteAsset();

	// Save data from FMapAsset to file on disk
	MapAsset->SaveMapData();
}

void FMap::OnWindowSizeChanged(FVector2D<int32> WindowSize)
{
}

FEntityManager* FMap::GetEntityManager() const
{
	return EntityManager;
}

int32 FMap::GetMapWidth() const
{
	return MapData.MapArray[0].Array.Size();
}

int32 FMap::GetMapHeight() const
{
	return MapData.MapArray.Size();
}

FVector2D<int32> FMap::GetMapSizeInTiles() const
{
	return { GetMapWidth(), GetMapHeight() };
}

FVector2D<int32> FMap::GetMapSizeInPixels() const
{
	return { GetMapWidth() * MapData.AssetsTileSize.X, GetMapHeight() * MapData.AssetsTileSize.Y };
}

void FMap::ReadAsset()
{
	if (MapAsset->IsMapDataValid())
	{
		MapData = MapAsset->GetMapData();

		bIsActive = true;
	}
}

void FMap::WriteAsset()
{
	MapAsset->WriteMapData(MapData);
}

FEntityManager* FMap::CreateEntityManager()
{
	return new FEntityManager(MapManager->GetOwnerWindow());
}

void FMap::UpdateMapOffsetOnRenderer() const
{
	FWindow* Window = MapManager->GetOwnerWindow();
	FRenderer* WindowRenderer = Window->GetRenderer();

	WindowRenderer->SetRenderOffset(MapRenderOffset);
}

bool FMap::IsInBounds(const FVector2D<int>& Location) const
{
	const FVector2D<int> MapSize = GetMapSizeInPixels();

	return (	(Location.X >= 0 && Location.X < MapSize.X)	&&
				(Location.Y >= 0 && Location.Y < MapSize.Y)	);
}

void FMap::AddMapRenderOffset(const FVector2D<int>& LocationChange)
{
	MapRenderOffset += LocationChange;

	const FVector2D<int> MapSize = GetMapSizeInPixels();
	const FVector2D<int> WindowSize = MapManager->GetOwnerWindow()->GetWindowSize();
	const FVector2D<int> HalfWindowSize = WindowSize / 2;

	// Width right limit
	if (MapRenderOffset.X > HalfWindowSize.X)
	{
		MapRenderOffset.X = HalfWindowSize.X;
	}
	// Width left limit
	else if (MapRenderOffset.X < -MapSize.X + HalfWindowSize.X)
	{
		MapRenderOffset.X = -MapSize.X + HalfWindowSize.X;
	}

	// Height down limit
	if (MapRenderOffset.Y > HalfWindowSize.Y)
	{
		MapRenderOffset.Y = HalfWindowSize.Y;
	}
	// Height up limit
	else if (MapRenderOffset.Y < -MapSize.Y + HalfWindowSize.Y)
	{
		MapRenderOffset.Y = -MapSize.Y + HalfWindowSize.Y;
	}

	// Trigger delegate for changed location
	MapLocationChangeDelegate.Execute(MapRenderOffset);

	UpdateMapOffsetOnRenderer();
}

FVector2D<int32> FMap::GetMapRenderOffset() const
{
	return MapRenderOffset;
}

FDelegate<void, FVector2D<int32>>& FMap::GetMapLocationChangeDelegate()
{
	return MapLocationChangeDelegate;
}

void FMap::ChangeTileAtLocation(const FVector2D<int32>& Location, const int32 MapAssetIndexToSet)
{
	const bool bDoesAssetIndexExists = MapData.MapSubAssetSettingsArray.IsValidIndex(MapAssetIndexToSet);

	if (bDoesAssetIndexExists)
	{
		const FVector2D<int> AbsoluteLocation = Location - MapRenderOffset;

		FVector2D<int> TileLocation;
		TileLocation.X = FMath::CeilToInt(static_cast<float>(AbsoluteLocation.X) / static_cast<float>(MapData.AssetsTileSize.X)) - 1;
		TileLocation.Y = FMath::CeilToInt(static_cast<float>(AbsoluteLocation.Y) / static_cast<float>(MapData.AssetsTileSize.Y)) - 1;

		if (MapData.MapArray.IsValidIndex(TileLocation.Y))
		{
			FMapRow& MapRow = MapData.MapArray[TileLocation.Y];

			if (MapRow.Array.IsValidIndex(TileLocation.X))
			{
				MapRow.Array[TileLocation.X] = MapAssetIndexToSet;
			}
		}
	}
	else
	{
		LOG_WARN("Location is out of bounds.");
	}
}
