#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "logging.h"
#include "render.h"


const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int TILE_SIZE = 40;


int main(int argc, char **argv) {

	// Init SDL

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}


	// Init SDL_image

	int img_init_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(img_init_flags) & img_init_flags) != img_init_flags) {
		logSDLError(std::cout, "IMG_Init");
		return 1;
	}

	// Create SDL_Window

	SDL_Window *win = SDL_CreateWindow("FCUK", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		return 1;
	}

	// Create SDL_Renderer

	SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		logSDLError(std::cout, "CreateRenderer");
		return 1;
	}


	SDL_Texture *bg = loadTexture("res/Chloe.jpg", renderer);
	SDL_Texture *image = loadTexture("res/smile.png", renderer);
	if (bg == nullptr || image == nullptr)
		return 4;


	// Render images

	SDL_RenderClear(renderer);

	int bW, bH;
	SDL_QueryTexture(bg, NULL, NULL, &bW, &bH);

	for(int i=0; i < SCREEN_WIDTH; i+=bW)
		for(int j=0; j < SCREEN_HEIGHT; j+=bH)
			renderTexture(bg, renderer, i, j);


	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
	renderTexture(image, renderer, x, y);

	SDL_RenderPresent(renderer);

	SDL_Delay(4000);


	// Cleanup

	SDL_DestroyTexture(bg);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();
	return 0;
}
