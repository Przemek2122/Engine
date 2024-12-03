// Engine.cpp : Defines the entry point for the application.

#include "Engine.h"
#include "SDL3/SDL.h"

using namespace std;

int SDL_main()
{
	cout << "Hello CMake." << endl;

	SDL_Window* Window = SDL_CreateWindow("tt", 100, 100, SDL_WINDOW_RESIZABLE);
	if (Window != nullptr)
	{
		cout << "Window created." << endl;
	}

	return 0;
}
