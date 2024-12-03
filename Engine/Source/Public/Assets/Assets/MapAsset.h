// Created by Przemys³aw Wiewióra 2023

#pragma once

#include "CoreMinimal.h"
#include "AssetBase.h"
#include "TextureAsset.h"
#include "Assets/Parser.h"

class FTextureAsset;

enum FMapIndex : int8_t
{
	INDEX_INCORRECT = -1
};

namespace FMapGlobalSettings
{
	/** Namespace for map files extensions and possible asset directory names */
	namespace Extensions
	{
		inline static const std::string PrimaryMapFileExtension = "amap";
		inline static const std::string MapDataFileExtension = "amapdata";
		inline static const CArray<std::string> AssetDirectoryNames = { "Assets", "assets" };
	}

	/** Namespace with comments added when saving a map */
	namespace FileComments
	{
		inline static const std::string MapTilesFileCommentLine1 = "This file is responsible for telling where is which tile located.";

		inline static const std::string MapAssetsFileCommentLine1 = "Map file";
		inline static const std::string MapAssetsFileCommentLine2 = "First number is asset number,";
		inline static const std::string MapAssetsFileCommentLine3 = "Second one is collision, where 0 is no collision and 1 means collision";
	}
}

struct FMapSubAssetSettings
{
	FMapSubAssetSettings();

	bool IsValid() const;

	void SetTextureAsset(FTextureAsset* InTextureAsset);

	FTextureAsset* GetTextureAsset() const;

	/** Index to match texture to map */
	int AssetIndex;

	/** Collision index, currently 0 and 1 supported */
	int Collision;

private:
	/** Asset pointer */
	FTextureAsset* TextureAsset;

};

struct FMapRow
{
	CArray<int> Array;
};

/** Map data: tiles size, tiles location and assets for map */
struct FMapData
{
	FMapData()
		: AssetsTileSize(32, 32)
	{
	}

	void Clear();

	/** Array with tile locations as ints */
	CArray<FMapRow> MapArray;

	/** Map assets array */
	CArray<FMapSubAssetSettings> MapSubAssetSettingsArray;

	/** Size of tile in pixels, for example 32x32 */
	FVector2D<int> AssetsTileSize;
};

/**
 * For loading storing and using font.
 */
class FMapAsset : public FAssetBase
{
public:
	FMapAsset(const std::string& InAssetName, const std::string& InAssetPath);
	virtual ~FMapAsset() override;

	virtual EAssetType GetAssetType() const override;

	/** Load assets from disk */
	void LoadMap();

	/** Clear all arrays from loaded files from FMapAsset::LoadMap */
	void ClearMapData();

	/** OVERRIDE file map source. This will remove old map. Be carefull when calling */
	void SaveMapData();

	bool IsMapDataValid() const;
	const FMapData& GetMapData() const;
	void WriteMapData(const FMapData& InMapData);

	void SetMapManager(FMapManager* InMapManager);

	bool IsLoaded() const;

protected:
	static FParser CreateMapFilesParser();

	/** Generate map assets names for two primary assets and for Assets directory location */
	void GenerateNamesForMapAssets();

	/** Load map assets into MapSubAssetSettingsArray */
	void LoadMapAssets(FParser& Parser, FAssetsManager* AssetsManager, SDL_Renderer* WindowRenderer);

	/** Load information about tiles location. */
	void LoadMapTilesLocationInformation(FParser& Parser);

	/** Save map tiles location */
	void SaveMapFile(FParser& Parser);

	/** Save map assets from MapSubAssetSettingsArray */
	void SaveMapDataFile(FParser& Parser);

protected:
	FMapManager* MapManager;

	bool bIsLoaded;

	/** Primary map file describing map look */
	std::string MapNameFilePath;
	/** Path for file describing Map assets */
	std::string MapDataFilePath;
	/** Path for map textures */
	std::string MapAssetsDirPath;

	/** Each string is line from MapNameFilePath */
	CArray<std::string> MapLines;

	/** Map data: tiles size, tiles location and assets for map */
	FMapData MapData;

};
