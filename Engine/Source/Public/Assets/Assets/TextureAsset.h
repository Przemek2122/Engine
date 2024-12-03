// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "AssetBase.h"
#include "Assets/TypesForAssets/Texture.h"

/**
 * Texture asset for reference to texture.
 * Without reference texture will be released.
 */
class FTextureAsset : public FAssetBase
{
public:
	FTextureAsset(const std::string& InAssetName, const std::string& InAssetPath);
	~FTextureAsset() override;

	/** Begin FAssetBase */
	EAssetType GetAssetType() const override;
	/** End FAssetBase */

	/** Returns size of texture. */
	FVector2D<float> GetSize() const;

	/** Get texture */
	FTexture* GetTexture() const;

	/** Call to create texture in SDL. Requires SDL_Renderer. */
	void PrepareTexture(SDL_Renderer* InRenderer);

	bool IsTexturePrepared() const;

protected:
	/** Texture - deleted in destructor */
	FTexture* Texture;

	bool bIsTexturePrepared;

};
