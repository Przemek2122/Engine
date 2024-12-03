// Created by Przemys³aw Wiewióra 2023

#include "CoreEngine.h"
#include "Assets/Assets/MapAsset.h"
#include "Assets/Assets/TextureAsset.h"
#include "Assets/Parser.h"
#include "Renderer/Map/MapManager.h"

FMapSubAssetSettings::FMapSubAssetSettings()
	: AssetIndex(INDEX_INCORRECT)
	, Collision(INDEX_INCORRECT)
	, TextureAsset(nullptr)
{
}

bool FMapSubAssetSettings::IsValid() const
{
	return (AssetIndex != INDEX_INCORRECT && Collision != INDEX_INCORRECT);
}

void FMapSubAssetSettings::SetTextureAsset(FTextureAsset* InTextureAsset)
{
	TextureAsset = InTextureAsset;
}

FTextureAsset* FMapSubAssetSettings::GetTextureAsset() const
{
	return TextureAsset;
}

void FMapData::Clear()
{
	MapArray.Clear();
	MapSubAssetSettingsArray.Clear();
}

FMapAsset::FMapAsset(const std::string& InAssetName, const std::string& InAssetPath)
	: FAssetBase(InAssetName, InAssetPath)
	, MapManager(nullptr)
	, bIsLoaded(false)
{
}

FMapAsset::~FMapAsset()
{
	ClearMapData();
}

EAssetType FMapAsset::GetAssetType() const
{
	return EAssetType::AT_MAP;
}

void FMapAsset::LoadMap()
{
	if (FFileSystem::Directory::Exists(AssetPath))
	{
		GenerateNamesForMapAssets();

		// Ensure all required files exists
		if (	FFileSystem::File::Exists(MapNameFilePath) 
			&&	FFileSystem::File::Exists(MapDataFilePath) 
			&&	FFileSystem::Directory::Exists(MapAssetsDirPath))
		{
			LOG_INFO("Loading map: " << AssetName);

			FParser Parser = CreateMapFilesParser();

			CArray<std::string> MapAssetFiles = FFileSystem::GetFilesFromDirectory(MapAssetsDirPath);

			FAssetsManager* AssetsManager = GEngine->GetAssetsManager();
			SDL_Renderer* WindowRenderer = MapManager->GetOwnerWindow()->GetRenderer()->GetSDLRenderer();

			LoadMapAssets(Parser, AssetsManager, WindowRenderer);

			LoadMapTilesLocationInformation(Parser);

			bIsLoaded = true;

			LOG_INFO("Map assets: " << MapData.MapSubAssetSettingsArray.Size() << " loaded, map name: " << AssetName);
		}
		else
		{
			LOG_ERROR("Missing map files for: " << AssetName);
		}
	}
	else
	{
		LOG_ERROR("Missing directory for map.");
	}
}

void FMapAsset::ClearMapData()
{
	MapNameFilePath.clear();
	MapDataFilePath.clear();
	MapAssetsDirPath.clear();
	MapLines.Clear();

	for (FMapSubAssetSettings& MapSubAssetSettings : MapData.MapSubAssetSettingsArray)
	{
		MapSubAssetSettings.GetTextureAsset()->DecrementNumberOfReferences();
	}

	MapData.Clear();

	bIsLoaded = false;
}

void FMapAsset::SaveMapData()
{
	// Create directory for map assets if it does not exist
	if (!FFileSystem::Directory::Exists(AssetPath))
	{
		FFileSystem::Directory::Create(AssetPath);

		LOG_INFO("Creating directory for assets: " << AssetPath);
	}

	if (FFileSystem::Directory::Exists(AssetPath))
	{
		FParser Parser = CreateMapFilesParser();

		SaveMapFile(Parser);

		SaveMapDataFile(Parser);
	}
	else
	{
		LOG_ERROR("Failed to create directory for assets: " << AssetPath);
	}
}

bool FMapAsset::IsMapDataValid() const
{
	return (MapData.MapArray.Size() > 0 && MapData.MapSubAssetSettingsArray.Size() > 0);
}

const FMapData& FMapAsset::GetMapData() const
{
	return MapData;
}

void FMapAsset::WriteMapData(const FMapData& InMapData)
{
	MapData = InMapData;
}

void FMapAsset::SetMapManager(FMapManager* InMapManager)
{
	MapManager = InMapManager;
}

bool FMapAsset::IsLoaded() const
{
	return bIsLoaded;
}

FParser FMapAsset::CreateMapFilesParser()
{
	return FParser({ ',' });
}

void FMapAsset::GenerateNamesForMapAssets()
{
	const std::string SharedBeginning = AssetPath + GEngine->GetAssetsManager()->GetPlatformSlash();
	const std::string SharedBeginningFiles = SharedBeginning + AssetName;

	MapNameFilePath = SharedBeginningFiles + '.' + FMapGlobalSettings::Extensions::PrimaryMapFileExtension;
	MapDataFilePath = SharedBeginningFiles + '.' + FMapGlobalSettings::Extensions::MapDataFileExtension;

	for (const std::string& AssetDirectoryName : FMapGlobalSettings::Extensions::AssetDirectoryNames)
	{
		const std::string Path = SharedBeginning + AssetDirectoryName;

		if (FFileSystem::Directory::Exists(Path))
		{
			MapAssetsDirPath = Path;

			break;
		}
	}
}

void FMapAsset::LoadMapAssets(FParser& Parser, FAssetsManager* AssetsManager, SDL_Renderer* WindowRenderer)
{
	std::ifstream MapDataFilePathStream(MapDataFilePath);

	for (std::string Line; getline(MapDataFilePathStream, Line); )
	{
		CArray<std::string> StringArray = Parser.SimpleParseLineIntoStrings(Line);
		if (StringArray.Size() == 3)
		{
			std::string SubAsset = StringArray[2];
			std::string SubAssetName = AssetName + SubAsset;

			char Slash = AssetsManager->GetPlatformSlash();
			std::string SubAssetAbsolutePath = MapAssetsDirPath + Slash + SubAsset;

			std::shared_ptr<FTextureAsset> Asset = AssetsManager->CreateAssetFromAbsolutePath<FTextureAsset>(SubAssetName, SubAssetAbsolutePath);
			AssetsManager->AddAssetExternal(Asset);

			if (Asset != nullptr)
			{
				MapData.MapSubAssetSettingsArray.Push(FMapSubAssetSettings());
				FMapSubAssetSettings& MapSubAssetSettings = MapData.MapSubAssetSettingsArray[MapData.MapSubAssetSettingsArray.Size() - 1];

				MapSubAssetSettings.SetTextureAsset(Asset.get());
				MapSubAssetSettings.AssetIndex = atoi(StringArray[0].c_str());
				MapSubAssetSettings.Collision = atoi(StringArray[1].c_str());

				Asset->PrepareTexture(WindowRenderer);
				Asset.get()->IncrementNumberOfReferences();
			}
			else
			{
				LOG_ERROR("Failed to load asset: " << SubAssetName << " from path: " << SubAssetAbsolutePath);
			}
		}
	}

	MapDataFilePathStream.close();
}

void FMapAsset::LoadMapTilesLocationInformation(FParser& Parser)
{
	const int MaxAssetIndex = MapData.MapSubAssetSettingsArray.Size();

	// Map - each tile location
	std::ifstream MapNameFilePathStream(MapNameFilePath);

	for (std::string Line; getline(MapNameFilePathStream, Line); )
	{
		CArray<std::string> ParsedArray = Parser.SimpleParseLineIntoStrings(Line);

		if (!ParsedArray.IsEmpty())
		{
			FMapRow MapRow;

			for (std::string& Element : ParsedArray)
			{
				// @TODO Could switch to strtol to make sure conversion does not fail
				const int Index = atoi(Element.c_str());

				if (Index >= MaxAssetIndex)
				{
					LOG_WARN("Found invalid index: '" << Index << "' Plase make sure asset for that index exists.");

					MapRow.Array.Push(INDEX_INCORRECT);
				}
				else
				{
					MapRow.Array.Push(Index);
				}
			}

			MapData.MapArray.Push(MapRow);
		}
	}

	MapNameFilePathStream.close();
}

void FMapAsset::SaveMapFile(FParser& Parser)
{
	// Create if not exists
	if (!FFileSystem::File::Exists(MapNameFilePath))
	{
		FFileSystem::File::Create(MapNameFilePath);
	}

	std::ofstream MapNameFileOfStream;

	// Open and clear content
	MapNameFileOfStream.open(MapNameFilePath, std::ofstream::out | std::ofstream::trunc);

	if (MapNameFileOfStream.is_open())
	{
		// Add comments at top of the file
		CArray<FParserLine> ParserLines;
		ParserLines.Push(FParserLine({ FParserText(FMapGlobalSettings::FileComments::MapTilesFileCommentLine1, Comment) }));

		for (FMapRow& MapRow : MapData.MapArray)
		{
			FParserLine ParserLine;

			for (const int MapElement : MapRow.Array)
			{
				int MapElementToSave = 0;

				// Make sure we do not have any negative values
				if (MapElement >= 0)
				{
					MapElementToSave = MapElement;
				}

				FParserText ParserText = FParserText(std::to_string(MapElementToSave), EParserTextType::Word);

				ParserLine.Texts.Push(std::move(ParserText));
			}

			ParserLines.Push(ParserLine);
		}

		// Convert array into string (@TODO This might be big - and is todo)
		const std::string StringToWrite = Parser.AdvancedParseLinesIntoString(ParserLines);

		// Write string to file
		MapNameFileOfStream.write(StringToWrite.c_str(), StringToWrite.size());
	}

	MapNameFileOfStream.close();
}

void FMapAsset::SaveMapDataFile(FParser& Parser)
{
	if (!FFileSystem::File::Exists(MapDataFilePath))
	{
		FFileSystem::File::Create(MapDataFilePath);
	}

	std::ofstream MapDataFileOfStream;

	// Open and clear content
	MapDataFileOfStream.open(MapDataFilePath, std::ofstream::out | std::ofstream::trunc);

	if (MapDataFileOfStream.is_open())
	{
		CArray<FParserLine> ParserLines;

		ParserLines.Push(FParserLine({ FParserText(FMapGlobalSettings::FileComments::MapAssetsFileCommentLine1, Comment) }));
		ParserLines.Push(FParserLine({ FParserText(FMapGlobalSettings::FileComments::MapAssetsFileCommentLine2, Comment) }));
		ParserLines.Push(FParserLine({ FParserText(FMapGlobalSettings::FileComments::MapAssetsFileCommentLine3, Comment) }));

		for (FMapSubAssetSettings& MapSubAssetSettingsArray : MapData.MapSubAssetSettingsArray)
		{
			FParserLine ParserLine;

			std::string AssetNameWithoutMapName = MapSubAssetSettingsArray.GetTextureAsset()->GetAssetName().substr(AssetName.size());

			ParserLine.Texts.Push(FParserText(std::to_string(MapSubAssetSettingsArray.AssetIndex), EParserTextType::Word));
			ParserLine.Texts.Push(FParserText(std::to_string(MapSubAssetSettingsArray.Collision), EParserTextType::Word));
			ParserLine.Texts.Push(FParserText(AssetNameWithoutMapName, EParserTextType::Word));

			ParserLines.Push(ParserLine);
		}

		const std::string StringToSave = Parser.AdvancedParseLinesIntoString(ParserLines);

		MapDataFileOfStream.write(StringToSave.c_str(), StringToSave.size());
	}

	MapDataFileOfStream.close();
}
