// Created by https://www.linkedin.com/in/przemek2122/ 2020

#pragma once

#include "CoreMinimal.h"

class FTexture
{
public:
	FTexture(const std::string& InTexturePath, SDL_Renderer* Renderer);
	~FTexture();

	void Draw(SDL_Renderer* Renderer, SDL_FRect SourceRect, SDL_FRect DestinationRect) const;

	SDL_Texture* GetSDLTexture() const;

protected:
	SDL_Texture* SDLTexture;
	
};
