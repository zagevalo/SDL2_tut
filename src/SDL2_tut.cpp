#include <iostream>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "logging.h"
#include "render.h"
#include "timer.h"


const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int TILE_SIZE = 40;
const int SPRITE_SIZE = 36;
const int VELOCITY = 10;


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


	SDL_Texture *bg = loadTexture("res/bg.png", renderer);
	SDL_Texture *character = loadTexture("res/Alice_spritesheet.png", renderer);
	if (bg == nullptr || character == nullptr)
		return 4;


	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	// Main Loop

	SDL_Rect char_clips[8][4];
	for (int i=0; i < 8; i++) {
		for (int j=0; j < 4; j++) {
			char_clips[i][j].x = j * SPRITE_SIZE;
			char_clips[i][j].y = i * SPRITE_SIZE;
			char_clips[i][j].w = SPRITE_SIZE;
			char_clips[i][j].h = SPRITE_SIZE;
		}
	}

	int direction_state = 4; // 0 - up, 1 - up_right, 2 - right, 3 - right_down, 4 - down,
							 // 5 - down_left, 6 - left, 7 - left_up

	int anim_frame = 0;

	SDL_Rect char_box;
	char_box.x = SCREEN_WIDTH / 2 - SPRITE_SIZE * 1.5;
	char_box.y = SCREEN_HEIGHT / 2 - SPRITE_SIZE * 1.5;
	char_box.w = SPRITE_SIZE * 3;
	char_box.h = SPRITE_SIZE * 3;

	SDL_Event e;
	bool quit = false;

	SDL_Color textColor = {0, 0, 0, 255};

	LTimer fpsTimer;
	int countedFrames = 0;
	std::stringstream fpsText;
	float avgFPS = 0;

	fpsTimer.start();


	while (!quit) {

		SDL_RenderClear(renderer);

		int bW, bH;
		SDL_QueryTexture(bg, NULL, NULL, &bW, &bH);

		// Calculate FPS
		avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
		if ( avgFPS > 2000000 )
			avgFPS = 0;

		SDL_Rect bg_box;
		bg_box.x = 0;
		bg_box.y = 0;
		bg_box.w = SCREEN_WIDTH;
		bg_box.h = SCREEN_HEIGHT;
		renderTexture(bg, renderer, bg_box, nullptr);

		//SDL_PumpEvents();
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
			if (e.type == SDL_MOUSEBUTTONDOWN)
				quit = true;
		}

		if (keys[SDL_SCANCODE_LEFT]) {
			if (char_box.x > 0)
				char_box.x -= VELOCITY;
				direction_state = 6;
				if (anim_frame >= 3)
					anim_frame = 0;
				else
					++anim_frame;
		}
		if (keys[SDL_SCANCODE_RIGHT]) {
			if (char_box.x < (SCREEN_WIDTH - SPRITE_SIZE * 4))
				char_box.x += VELOCITY;
				direction_state = 2;
				if (anim_frame >= 3)
					anim_frame = 0;
				else
					++anim_frame;
		}
		if (keys[SDL_SCANCODE_UP]) {
			if (char_box.y > 0)
				char_box.y -= VELOCITY;
				direction_state = 0;
				if (anim_frame >= 3)
					anim_frame = 0;
				else
					++anim_frame;
		}
		if (keys[SDL_SCANCODE_DOWN]) {
			if (char_box.y < (SCREEN_HEIGHT - SPRITE_SIZE * 4))
				char_box.y += VELOCITY;
				direction_state = 4;
				if (anim_frame >= 3)
					anim_frame = 0;
				else
					++anim_frame;
		}
		if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_RIGHT]) {
			if ((char_box.y > 0) && (char_box.x < (SCREEN_WIDTH - SPRITE_SIZE * 4))) {
				char_box.x += VELOCITY / 2;
				char_box.y -= VELOCITY / 2;
			}
			direction_state = 1;
			if (anim_frame >= 3)
				anim_frame = 0;
			else
				++anim_frame;
		}
		if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_LEFT]) {
			if ((char_box.y > 0) && (char_box.x > 0)) {
				char_box.x -= VELOCITY / 2;
				char_box.y -= VELOCITY / 2;
			}
			direction_state = 5;
			if (anim_frame >= 3)
				anim_frame = 0;
			else
				++anim_frame;
		}
		if (keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_RIGHT]) {
			if ((char_box.y < (SCREEN_HEIGHT - SPRITE_SIZE * 4)) &&
					(char_box.x < (SCREEN_WIDTH - SPRITE_SIZE * 4))) {
				char_box.x += VELOCITY / 2;
				char_box.y += VELOCITY / 2;
			}
			direction_state = 3;
			if (anim_frame >= 3)
				anim_frame = 0;
			else
				++anim_frame;
		}
		if (keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_LEFT]) {
			if ((char_box.y < (SCREEN_HEIGHT - SPRITE_SIZE * 4)) && (char_box.x > 0)) {
				char_box.x -= VELOCITY / 2;
				char_box.y += VELOCITY / 2;
			}
			direction_state = 7;
			if (anim_frame >= 3)
				anim_frame = 0;
			else
				++anim_frame;
		}
		if (keys[SDL_SCANCODE_ESCAPE]) {
				quit = true;
		}

		renderTexture(character, renderer, char_box, &char_clips[direction_state][anim_frame]);

		SDL_RenderPresent(renderer);
	}


	// Cleanup

	SDL_DestroyTexture(bg);
	SDL_DestroyTexture(character);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();
	return 0;
}
