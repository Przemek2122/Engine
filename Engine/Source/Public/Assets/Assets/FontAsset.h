// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "AssetBase.h"

/**
 * For loading storing and using font.
 */
class FFontAsset : public FAssetBase
{
public:
	FFontAsset(const std::string& InAssetName, const std::string& InAssetPath);
	virtual ~FFontAsset() override;

	EAssetType GetAssetType() const override;
	
	_NODISCARD FFont* GetFont(const int Size);
	
protected:
	void MakeFont(const int Size);

protected:
	/** Different font sizes */
	CMap<int, FFont*> Fonts;

};
