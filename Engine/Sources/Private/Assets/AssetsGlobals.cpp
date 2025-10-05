// Created by Przemys�aw Wiewi�ra 2020-2025

#include "CoreEngine.h"
#include "Assets/AssetsGlobals.h"

std::string FAssetsGlobals::GetAssetReadType(const EAssetReadMethod AssetReadType, const bool bUseFileAsBinary)
{
	std::string OutValue;

	switch (AssetReadType)
	{
		case EAssetReadMethod::OpenForReading:
		{
			OutValue = "r";

			break;
		}
		case EAssetReadMethod::CreateEmptyFile:
		{
			OutValue = "w";

			break;
		}
		case EAssetReadMethod::AppendToFile:
		{
			OutValue = "a";

			break;
		}
		case EAssetReadMethod::OpenFile:
		{
			OutValue = "r+";

			break;
		}
		case EAssetReadMethod::CreateEmptyFileForReadingAndWriting:
		{
			OutValue = "w+";

			break;
		}
		case EAssetReadMethod::OpenFileForReadingAndAppending:
		{
			OutValue = "a+";

			break;
		}
	}

	if (bUseFileAsBinary)
	{
		OutValue += "b";
	}

	return std::move(OutValue);
}
