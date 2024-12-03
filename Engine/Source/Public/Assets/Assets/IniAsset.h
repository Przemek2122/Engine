// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "AssetBase.h"
#include "Assets/Parser.h"

/**
 * For loading storing and using font.
 */
class FIniAsset : public FAssetBase
{
public:
	FIniAsset(const std::string& InAssetName, const std::string& InAssetPath);
	~FIniAsset() override;

	/** Begin FAssetBase */
	EAssetType GetAssetType() const override;
	/** End FAssetBase */

	std::string GetAbsolutePath() const;

	bool DoesFileExist() const;

	void Load(FParser* IniParser);
	void Save(FParser* IniParser);

	/** Clear content without saving */
	void UnLoad();

	const CArray<FParserLine>& GetLines() const;

	void SetLines(const CArray<FParserLine>& NewLines);

protected:
	CArray<FParserLine> Lines;

};
