// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"
#include "Renderer/Map/Map.h"
#include "Assets/Assets/TextureAsset.h"
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
}

void FMap::DeInitialize()
{
	ClearData();

	if (EntityManager != nullptr)
	{
		EntityManager->DestroyEntity(CameraManagerEntity);

		delete EntityManager;
	}
}

FEntityManager* FMap::GetEntityManager() const
{
	return EntityManager;
}

int FMap::GetMapWidth() const
{
	return MapData.MapArray[0].Array.Size();
}

int FMap::GetMapHeight() const
{
	return MapData.MapArray.Size();
}

FVector2D<int> FMap::GetMapSizeInTiles() const
{
	return { GetMapWidth(), GetMapHeight() };
}

FVector2D<int> FMap::GetMapSizeInPixels() const
{
	return { GetMapWidth() * MapData.AssetsTileSize.X, GetMapHeight() * MapData.AssetsTileSize.Y };
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
		const FVector2D<float> OwnerWindowSize = MapManager->GetOwnerWindow()->GetWindowSize();
		const FVector2D<float> MapLocationFloat = MapRenderOffset;
		const FVector2D<float> MapAssetsTileSizeFloat = MapData.AssetsTileSize;

		// Minimal render tile offset - Everything before that vector will not be rendered
		MapLocationTileOffsetMin.X = FMath::FloorToInt(-MapLocationFloat.X / MapAssetsTileSizeFloat.X);
		MapLocationTileOffsetMin.Y = FMath::FloorToInt(-MapLocationFloat.Y / MapAssetsTileSizeFloat.Y);

		MapLocationTileOffsetMin.X = FMath::Max(MapLocationTileOffsetMin.X, 0);
		MapLocationTileOffsetMin.Y = FMath::Max(MapLocationTileOffsetMin.Y, 0);

		// Max render tile offset - Everything after that vector will not be rendered
		MapLocationTileOffsetMax = MapLocationTileOffsetMin;
		MapLocationTileOffsetMax.X += FMath::CeilToInt(OwnerWindowSize.X / MapAssetsTileSizeFloat.X);
		MapLocationTileOffsetMax.Y += FMath::CeilToInt(OwnerWindowSize.Y / MapAssetsTileSizeFloat.Y);

		SDL_Renderer* WindowRenderer = MapManager->GetOwnerWindow()->GetRenderer()->GetSDLRenderer();

		SDL_FRect Source, Destination;
		Source.x = 0;
		Source.y = 0;
		Source.h = MapData.AssetsTileSize.Y;
		Source.w = MapData.AssetsTileSize.X;
		Destination.h = MapData.AssetsTileSize.Y;
		Destination.w = MapData.AssetsTileSize.X;

		const int VerticalArraySize = MapData.MapArray.Size();

		for (int VerticalIndex = 0; VerticalIndex < VerticalArraySize; VerticalIndex++)
		{
			// Optimization - Do not render offscreen
			if (VerticalIndex >= MapLocationTileOffsetMin.Y && VerticalIndex <= MapLocationTileOffsetMax.Y)
			{
				const FMapRow& MapRow = MapData.MapArray[VerticalIndex];

				const int HorizontalArraySize = MapRow.Array.Size();

				for (int HorizontalIndex = 0; HorizontalIndex < HorizontalArraySize; HorizontalIndex++)
				{
					// Optimization - Do not render offscreen
					if (HorizontalIndex >= MapLocationTileOffsetMin.X && HorizontalIndex <= MapLocationTileOffsetMax.X)
					{
						const int AssetIndex = MapRow.Array[HorizontalIndex];

						if (AssetIndex >= 0)
						{
							const FMapSubAssetSettings CurrentAssetSettings = MapData.MapSubAssetSettingsArray[AssetIndex];

							Destination.x = MapRenderOffset.X + HorizontalIndex * MapData.AssetsTileSize.X;
							Destination.y = MapRenderOffset.Y + VerticalIndex * MapData.AssetsTileSize.Y;

							CurrentAssetSettings.GetTextureAsset()->GetTexture()->Draw(WindowRenderer, Source, Destination);
						}
						else
						{
							// Do sth when index is not found

							// @TODO ...
						}
					}
				}
			}
		}

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

FVector2D<int> FMap::GetMapRenderOffset() const
{
	return MapRenderOffset;
}

FDelegate<void, FVector2D<int>>& FMap::GetMapLocationChangeDelegate()
{
	return MapLocationChangeDelegate;
}

void FMap::ChangeTileAtLocation(const FVector2D<int>& Location, const int MapAssetIndexToSet)
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
