// Created by Przemys³aw Wiewióra 2020-2024

#pragma once

#include "BaseAssetsCollection.h"

/**
 * Engine collection for asset paths
 */
class FEngineAssetsCollection : public FBaseAssetsCollection
{
public:
	inline static const FAssetCollectionItem UIButton1 = { "UIButton1", R"(Assets\Textures\UI\Button_400_80\1.png)" };
	inline static const FAssetCollectionItem ArrowTransparent1 = { "ArrowTransparent1", R"(Assets\Textures\Arrow\Transparent1.png)" };

};
