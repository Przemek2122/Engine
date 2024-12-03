// Created by Przemys³aw Wiewióra 2020-2024

#pragma once

/**
 * Base class for all asset collection items - Will keep AssetName and AssetPath.
 */
struct FAssetCollectionItem
{
protected:
	std::string AssetName;
	std::string AssetPath;

public:
	FAssetCollectionItem()
	{
	}

	FAssetCollectionItem(std::string InAssetName, std::string InAssetPath);

	const std::string& GetAssetName() const;
	const std::string& GetAssetPath() const;

};
