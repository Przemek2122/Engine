// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

enum class EAssetType : uint8_t
{
	/** None type - Will be used for searching all assets. */
	AT_NONE = 0,
	/** Default type - Will be used if function GetAssetType is not overriden. */
	AT_BASE = 1,
	AT_TEXTURE,
	AT_FONT,
	AT_AUDIO,
	AT_MAP,
	AT_INI,
};

/**
 * Base class for any asset type files.
 */
class FAssetBase
{
	friend FEngine;
	
public:
	FAssetBase(std::string InAssetName, std::string InAssetPath);
	virtual ~FAssetBase();

	virtual EAssetType GetAssetType() const;

	_NODISCARD virtual std::string GetAssetName() const;
	_NODISCARD virtual std::string GetAssetPath() const;

	/** @returns number of  */
	int GetNumberOfReferences() const;

	/** Adds one to number of referenced asset, use for automatic release */
	void IncrementNumberOfReferences();

	/** Removes one to number of referenced asset, use for automatic release */
	void DecrementNumberOfReferences();

protected:
	/** Called when number of referenced changed to lower one */
	virtual void OnNumberOfReferencesLowered();

	/** Called when there are no references */
	virtual void ReleaseAsset();

protected:
	std::string AssetName;
	std::string AssetPath;

	int NumberOfReferences;
	
};
