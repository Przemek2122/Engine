// Created by Przemys³aw Wiewióra 2020-2024

#include "CoreEngine.h"
#include "Assets/Collection/AssetCollectionItem.h"

FAssetCollectionItem::FAssetCollectionItem(std::string InAssetName, std::string InAssetPath)
	: AssetName(std::move(InAssetName))
	, AssetPath(std::move(InAssetPath))
{
}

const std::string& FAssetCollectionItem::GetAssetName() const
{
	return AssetName;
}

const std::string& FAssetCollectionItem::GetAssetPath() const
{
	return AssetPath;
}
