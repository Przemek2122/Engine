// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "AssetsManager.h"

/**
 * Class for helping use of AssetsManager.
 */
class FAssetsManagerHelpers
{
public:
	/**
	 * Find or create asset. It will try to return existing asset. If it does not exist it will create one.
	 * To manage asset lifecycle after getting it use functions:
	 * FAssetBase::IncrementNumberOfReferences() and FAssetBase::DecrementNumberOfReferences()
	 * If used properly, memory (for given assets) will be freed and allocated based on need.
	 */
	template<typename TAssetSubClass>
	static TAssetSubClass* GetOrCreateAsset(FAssetsManager* InAssetsManager, FWindow* Window, const std::string& InAssetName, const std::string& InAssetPath, const EAssetType OptionalAssetType = EAssetType::AT_NONE)
	{
		bool bHasAsset = false;
		TAssetSubClass* TextureAsset = nullptr;

		if (InAssetsManager->HasAsset<TAssetSubClass>(InAssetName, EAssetType::AT_TEXTURE))
		{
			TextureAsset = InAssetsManager->GetAsset<TAssetSubClass>(InAssetName, OptionalAssetType);

			if (TextureAsset != nullptr)
			{
				bHasAsset = true;
			}
		}
		else if (!InAssetPath.empty())
		{
			TextureAsset = InAssetsManager->AddAsset<TAssetSubClass>(InAssetName, InAssetPath);

			if (TextureAsset != nullptr)
			{
				TextureAsset->PrepareTexture(Window->GetRenderer()->GetSDLRenderer());

				bHasAsset = true;
			}
		}

		if (bHasAsset)
		{
			return TextureAsset;
		}
		else
		{
			return nullptr;
		}
	}


};
