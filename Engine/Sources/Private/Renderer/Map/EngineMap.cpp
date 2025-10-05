// Created by Przemys³aw Wiewióra 2025

#include "CoreEngine.h"
#include "Renderer/Map/EngineMap.h"

#include "Renderer/Map/MapManager.h"

FEngineMap::FEngineMap(FMapAsset* InMapAsset, FMapManager* InMapManager) : FMap(InMapAsset, InMapManager)
{
}

void FEngineMap::Render()
{
	FMap::Render();

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

		RenderSubSystems();
	}
}
