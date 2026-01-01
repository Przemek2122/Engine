// Created by https://www.linkedin.com/in/przemek2122/ 2020

#include "CoreEngine.h"
#include "Assets/TypesForAssets/Texture.h"

FTexture::FTexture(const std::string& InTexturePath, SDL_Renderer* Renderer)
{
#if defined(ENGINE_USING_VIDEO) && ENGINE_USING_VIDEO
	// Attempt load
	SDL_Surface* TemporarySurface = IMG_Load(InTexturePath.c_str());

	// Check if succesfuly loaded
	if (TemporarySurface != nullptr)
	{
		// To memory
		SDLTexture = SDL_CreateTextureFromSurface(Renderer, TemporarySurface);

		SDL_DestroySurface(TemporarySurface);
	}
	else
	{
		LOG_ERROR("Can not load texture: " << InTexturePath << " ! (" << SDL_GetError() << ")");
	}
#endif
}

FTexture::~FTexture()
{
#if defined(ENGINE_USING_VIDEO) && ENGINE_USING_VIDEO
	if (SDLTexture != nullptr)
	{
		SDL_DestroyTexture(SDLTexture);
	}
	else
	{
		LOG_WARN("Texture not destroyed (pointer invalid)!");
	}
#endif
}

void FTexture::Draw(SDL_Renderer* Renderer, const SDL_FRect SourceRect, const SDL_FRect DestinationRect) const
{
#if defined(ENGINE_USING_VIDEO) && ENGINE_USING_VIDEO
	SDL_RenderTexture(Renderer, SDLTexture, &SourceRect, &DestinationRect);
#endif
}

SDL_Texture* FTexture::GetSDLTexture() const
{
	return SDLTexture;
}
