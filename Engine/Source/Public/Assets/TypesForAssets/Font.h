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

	_NODISCARD TTF_Font* GetFont() const;
	_NODISCARD int GetFontSize() const;

	void Reinitialize();

	_NODISCARD std::string GetFontAssetName() const;
	
protected:
	virtual void InitializeFont();
	virtual void DeInitializeFont();

	TTF_Font *Font;
	
	FFontAsset* FontAsset;
	int FontSize;
	
};
