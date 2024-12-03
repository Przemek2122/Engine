// Created by Przemysław Wiewióra 2024

#pragma once

enum class EEngineErrorCode
{
	/** Successful output */
	Success = 0,
	/** Generic force exit */
	ForceExit = -1,

	SDL_InitFail = -2,
	TTF_InitFail = -3,
	Mix_InitFail = -4,
	Mixer_OpenAudioFailed = -5,
	CanNotCreateRenderer = -10,
	CanNotCreateWindow = -11,

};
