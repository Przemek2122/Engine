// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Assets/TypesForAssets/Font.h"

#include "Assets/Assets/FontAsset.h"

FFont::FFont(FFontAsset* InFontAsset, const int InFontSize)
	: Font(nullptr)
	, FontAsset(InFontAsset)
	, FontSize(InFontSize)
{
	FFont::InitializeFont();

#if ENGINE_MEMORY_ALLOCATION_DEBUG_FONTS
	LOG_INFO("Created font: " << FontAsset << "@" << FontSize);
#endif
}

FFont::~FFont()
{
	FFont::DeInitializeFont();

#if ENGINE_MEMORY_ALLOCATION_DEBUG_FONTS
	LOG_INFO("Destroyed font: " << FontAsset << "@" << FontSize);
#endif
}

TTF_Font* FFont::GetFont() const
{
	return Font;
}

int FFont::GetFontSize() const
{
	return FontSize;
}

void FFont::Reinitialize()
{
	Font = nullptr;
}
std::string FFont::GetFontAssetName() const
{
	return FontAsset->GetAssetName();
}

void FFont::InitializeFont()
{
	if (FFileSystem::File::Exists(FontAsset->GetAssetPath()))
	{
		Font = TTF_OpenFont(FontAsset->GetAssetPath().c_str(), FontSize);
		if (Font == nullptr)
		{
			LOG_ERROR("TTF_OpenFont: %s\n" << SDL_GetError());
		}
	}
	else
	{
		LOG_ERROR("Font file does not exist: " << FontAsset->GetAssetPath());
	}
}

void FFont::DeInitializeFont()
{
	TTF_CloseFont(Font);
}
