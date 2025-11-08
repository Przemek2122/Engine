// Created by https://www.linkedin.com/in/przemek2122/ 2020

#pragma once

#include "CoreMinimal.h"
#include "AssetBase.h"

/**
 * For loading storing and using font.
 */
class ENGINE_API FFontAsset : public FAssetBase
{
public:
	FFontAsset(const std::string& InAssetName, const std::string& InAssetPath);
	virtual ~FFontAsset() override;

	EAssetType GetAssetType() const override;
	
	NO_DISCARD FFont* GetFont(const int Size);
	
protected:
	void MakeFont(const int Size);

protected:
	/** Different font sizes */
	CMap<int, FFont*> Fonts;

};
