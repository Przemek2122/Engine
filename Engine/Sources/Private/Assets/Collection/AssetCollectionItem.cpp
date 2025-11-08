// Created by https://www.linkedin.com/in/przemek2122/ 2020-2024

#include "CoreEngine.h"
#include "Assets/Collection/AssetCollectionItem.h"

FAssetCollectionItem::FAssetCollectionItem(std::string InAssetName, const std::string& InAssetPath)
	: AssetName(std::move(InAssetName))
	, AssetPath(InAssetPath)
{
}

const std::string& FAssetCollectionItem::GetAssetName() const
{
	return AssetName;
}

std::string FAssetCollectionItem::GetAssetPath() const
{
	return AssetPath.string();
}
