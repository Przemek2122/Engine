// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"
#include "Assets/AssetsGlobals.h"

std::string FAssetsGlobals::GetAssetReadType(const EAssetReadType AssetReadType, const bool bUseFileAsBinary)
{
	std::string OutValue;

	switch (AssetReadType)
	{
		case EAssetReadType::OpenForReading:
		{
			OutValue = "r";

			break;
		}
		case EAssetReadType::CreateEmptyFile:
		{
			OutValue = "w";

			break;
		}
		case EAssetReadType::AppendToFile:
		{
			OutValue = "a";

			break;
		}
		case EAssetReadType::OpenFile:
		{
			OutValue = "r+";

			break;
		}
		case EAssetReadType::CreateEmptyFileForReadingAndWriting:
		{
			OutValue = "w+";

			break;
		}
		case EAssetReadType::OpenFileForReadingAndAppending:
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
