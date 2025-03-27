// Created by Przemys³aw Wiewióra 2020-2024

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
	 * (USE '/' to FORWARD SLASH)
	 * Sample: 'Textures/UI/Squares/Red.jpg'
	 * @Note: Do not add Assets dir
	 */
	std::filesystem::path AssetPath;

public:
	FAssetCollectionItem() = default;
	FAssetCollectionItem(std::string InAssetName, const std::string& InAssetPath);

	const std::string& GetAssetName() const;
	std::string GetAssetPath() const;

};
