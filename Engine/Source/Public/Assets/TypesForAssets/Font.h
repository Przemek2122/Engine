// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

/**
 * For loading storing and using font.
 */
class FFont
{
public:
	FFont(FFontAsset* InFontAsset, const int InFontSize);
	virtual ~FFont();

	NO_DISCARD TTF_Font* GetFont() const;
	NO_DISCARD int GetFontSize() const;

	void Reinitialize();

	NO_DISCARD std::string GetFontAssetName() const;
	
protected:
	virtual void InitializeFont();
	virtual void DeInitializeFont();

	TTF_Font *Font;
	
	FFontAsset* FontAsset;
	int FontSize;
	
};
