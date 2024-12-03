// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "Assets/Assets/IniAsset.h"

#include "Assets/Parser.h"
#include "Assets/IniReader/IniManager.h"

FIniAsset::FIniAsset(const std::string& InAssetName, const std::string& InAssetPath)
	: FAssetBase(InAssetName, InAssetPath)
{
}

FIniAsset::~FIniAsset()
{
}

EAssetType FIniAsset::GetAssetType() const
{
	return EAssetType::AT_INI;
}

std::string FIniAsset::GetAbsolutePath() const
{
	return GEngine->GetAssetsManager()->ConvertRelativeToFullPath(AssetPath);
}

bool FIniAsset::DoesFileExist() const
{
	return FFileSystem::File::Exists(AssetPath);
}

void FIniAsset::Load(FParser* IniParser)
{
	if (IniParser != nullptr && DoesFileExist())
	{
		FDelegateSafe<void, const std::string&> FileReaderDelegate;
		FileReaderDelegate.BindLambda([&](const std::string& Line)
		{
			FParserLine ParserLines = IniParser->AdvancedParseStringIntoLines(Line);

			Lines.Push(ParserLines);
		});

		FFileSystem::File::GetFileContentLineByLine(FileReaderDelegate, AssetPath);
	}
}

void FIniAsset::Save(FParser* IniParser)
{
	if (IniParser != nullptr)
	{
		
	}
}

void FIniAsset::UnLoad()
{
}

const CArray<FParserLine>& FIniAsset::GetLines() const
{
	return Lines;
}

void FIniAsset::SetLines(const CArray<FParserLine>& NewLines)
{
	Lines = NewLines;
}
