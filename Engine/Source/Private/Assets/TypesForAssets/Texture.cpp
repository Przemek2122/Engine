// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Assets/TypesForAssets/Texture.h"
#include "SDL3/SDL_image.h"

FTexture::FTexture(const std::string& InTexturePath, SDL_Renderer* Renderer)
{
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
}

FTexture::~FTexture()
{
	if (SDLTexture != nullptr)
	{
		SDL_DestroyTexture(SDLTexture);
	}
	else
	{
		LOG_WARN("Texture not destroyed (pointer invalid)!");
	}
}

void FTexture::Draw(SDL_Renderer* Renderer, const SDL_FRect SourceRect, const SDL_FRect DestinationRect) const
{
	SDL_RenderTexture(Renderer, SDLTexture, &SourceRect, &DestinationRect);
}

SDL_Texture* FTexture::GetSDLTexture() const
{
	return SDLTexture;
}
