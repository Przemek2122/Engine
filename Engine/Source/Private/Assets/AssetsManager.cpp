// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Assets/AssetsManager.h"
#include "Assets/Assets/AssetBase.h"
#include "Assets/IniReader/IniManager.h"

FAssetsManager::FAssetsManager()
	: IniManager(new FIniManager(this))
	, AssetDirName("Assets")
	, ConfigDirName("Config")
	, MapsDirName("Maps")
	, FontsDirName("Fonts")
{
}

FAssetsManager::~FAssetsManager()
{
	for (auto& [AssetType, Assets] : AssetsByType)
	{
		Assets.AssetsMap.Clear();
	}

	AssetsByType.Clear();
}

FIniManager* FAssetsManager::GetIniManager() const
{
	return IniManager;
}

void FAssetsManager::RemoveAsset(const std::string& InAssetName, const EAssetType OptionalAssetType)
{
	if (OptionalAssetType == EAssetType::AT_NONE)
	{
		for (std::pair<const EAssetType, FAssetsStructure>& ByType : AssetsByType)
		{
			ByType.second.AssetsMap.Remove(InAssetName);
		}
	}
	else
	{
		AssetsByType[OptionalAssetType].AssetsMap.Remove(InAssetName);
	}
}

std::shared_ptr<FAssetBase> FAssetsManager::GetAsset(const std::string& InAssetName, const EAssetType OptionalAssetType)
{
	// Search all assets
	if (OptionalAssetType == EAssetType::AT_NONE)
	{
		for (const std::pair<const EAssetType, FAssetsStructure>& ByType : AssetsByType)
		{
			if (ByType.second.AssetsMap.ContainsKey(InAssetName))
			{
				return ByType.second.AssetsMap.At(InAssetName);
			}
		}
	}
	// Search specified type
	else
	{
		if (AssetsByType[OptionalAssetType].AssetsMap.ContainsKey(InAssetName))
		{
			return AssetsByType[OptionalAssetType].AssetsMap.At(InAssetName);
		}
	}

	return nullptr;
}

CArray<std::string> FAssetsManager::GetFilesFromDirectory(const std::string& Directory) const
{
	return FFileSystem::GetDirectories(ConvertRelativeToFullPath(Directory), true);
}

std::string FAssetsManager::GetProjectLocation() const
{
	return FGlobalDefines::GEngine->GetLaunchRelativePath();
}

char FAssetsManager::GetPlatformSlash() const
{
	return FFileSystem::GetPlatformSlash();
}

bool FAssetsManager::ExistsFileOrDirectory(const std::string& InAssetPath)
{
	return (FFileSystem::File::Exists(InAssetPath) || FFileSystem::Directory::Exists(InAssetPath));
}

std::string FAssetsManager::GetAssetsPathRelative() const
{
	return AssetDirName;
}

std::string FAssetsManager::GetConfigPathRelative() const
{
	return ConfigDirName;
}

std::string FAssetsManager::GetMapsPathRelative() const
{
	return MapsDirName;
}

std::string FAssetsManager::GetFontsPathRelative() const
{
	return GetAssetsPathRelative() + FontsDirName;
}

std::string FAssetsManager::ConvertRelativeToFullPath(const std::string& InPathRelative) const
{
	std::string OutPath = FGlobalDefines::GEngine->GetLaunchRelativePath();

#if !PLATFORM_ANDROID
	OutPath += AssetDirName + FFileSystem::GetPlatformSlash();
#endif

	OutPath += InPathRelative;

	return OutPath;
}
