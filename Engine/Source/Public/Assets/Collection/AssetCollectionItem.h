// Created by Przemys�aw Wiewi�ra 2020-2024

#pragma once

/**
 * Base class for all asset collection items - Will keep AssetName and AssetPath.
 */
struct ENGINE_API FAssetCollectionItem
{
protected:
	/** Unique name for the asset, should be unique across all asset types */
	std::string AssetName;

	/**
	 * Relative path to asset
	 * Sample: 'Textures\UI\Squares\Red.jpg'
	 * @Note: Do not add Assets dir
	 */
	std::string AssetPath;

public:
	FAssetCollectionItem() = default;
	FAssetCollectionItem(std::string InAssetName, std::string InAssetPath);

	const std::string& GetAssetName() const;
	const std::string& GetAssetPath() const;

};
