#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "SDL_Routine.h"
#include "../logging/logging.h"
#include "../consts.h"
#include <iostream>

SDL_Routine::SDL_Routine() {
	MainWind = nullptr;
	Render = nullptr;
}

int SDL_Routine::InitSDL() {

	int ret=0;

	// Init SDL

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "SDL_Init");
		ret = 1;
	}


	// Init SDL_image

	int img_init_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(img_init_flags) & img_init_flags) != img_init_flags) {
		logSDLError(std::cout, "IMG_Init");
		ret = 1;
	}


	// Init SDL_ttf

	if (TTF_Init() != 0) {
		logSDLError(std::cout, "TTF_Init");
		ret = 1;
	}


	// Init SDL_mixer

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) != 0) {
		logSDLError(std::cout, "Mix_OpenAudio");
		ret = 1;
	}


	// Create SDL_Window

	MainWind = SDL_CreateWindow("FCUK", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (MainWind == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		ret = 1;
	}

	// Create SDL_Renderer

	Render = SDL_CreateRenderer(MainWind, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Render == nullptr) {
		logSDLError(std::cout, "CreateRenderer");
		ret = 1;
	}

	return ret;

}


int SDL_Routine::CleanupSDL() {
	SDL_DestroyRenderer(Render);
	SDL_DestroyWindow(MainWind);

	SDL_Quit();

	return 0;
}
