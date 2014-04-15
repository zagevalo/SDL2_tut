#include <iostream>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "logging.h"
#include "render.h"
#include "timer.h"


const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int TILE_SIZE = 40;
const int SPRITE_SIZE = 36;
const int ANIMATION_FRAMES = 4;
const int VELOCITY = 4;


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


	// Init SDL_ttf

	if (TTF_Init() != 0) {
		logSDLError(std::cout, "TTF_Init");
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

	SDL_Rect char_clips[4][8];
	for (int i=0; i < 8; i++) {
		for (int j=0; j < 4; j++) {
			char_clips[j][i].x = j * SPRITE_SIZE;
			char_clips[j][i].y = i * SPRITE_SIZE;
			char_clips[j][i].w = SPRITE_SIZE;
			char_clips[j][i].h = SPRITE_SIZE;
		}
	}

	int direction_state = 4; // 0 - up, 1 - up_right, 2 - right, 3 - right_down, 4 - down,
							 // 5 - down_left, 6 - left, 7 - left_up

	int anim_frame = 0;
	int frame = 0;

	SDL_Rect char_box;
	char_box.x = SCREEN_WIDTH / 2 - SPRITE_SIZE * 1.5;
	char_box.y = SCREEN_HEIGHT / 2 - SPRITE_SIZE * 1.5;
	char_box.w = SPRITE_SIZE * 3;
	char_box.h = SPRITE_SIZE * 3;

	SDL_Event e;
	bool quit = false;

	TTF_Font* fps_font = openFont("res/beau.ttf", 25);
	SDL_Color textColor = {0, 0, 0};

	std::stringstream fps_text;
	SDL_Texture* txt_image;
	//SDL_Texture* txt_image = renderText("FPS test text", fps_font, textColor, renderer);
	//if (txt_image == nullptr) {
	//	return 1;
	//}

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
		fps_text.str("");
		fps_text << "FPS: " << avgFPS;

		// Render BG
		SDL_Rect bg_box;
		bg_box.x = 0;
		bg_box.y = 0;
		bg_box.w = SCREEN_WIDTH;
		bg_box.h = SCREEN_HEIGHT;
		renderTexture(bg, renderer, bg_box, nullptr);

		// Show FPS
		txt_image = renderText(fps_text.str().c_str(), fps_font, textColor, renderer);
		if (txt_image == nullptr) {
			return 1;
		}

		//SDL_PumpEvents();
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
			if (e.type == SDL_MOUSEBUTTONDOWN)
				quit = true;
		}

		if (keys[SDL_SCANCODE_LEFT]) {
			direction_state = 6;
			anim_frame = frame / 4;
			if (char_box.x > 0)
				char_box.x -= VELOCITY;
		}
		if (keys[SDL_SCANCODE_RIGHT]) {
			direction_state = 2;
			anim_frame = frame / 4;
			if (char_box.x <= (SCREEN_WIDTH - SPRITE_SIZE * 3))
				char_box.x += VELOCITY;
		}
		if (keys[SDL_SCANCODE_UP]) {
			direction_state = 0;
			anim_frame = frame / 4;
			if (char_box.y > 0)
				char_box.y -= VELOCITY;
		}
		if (keys[SDL_SCANCODE_DOWN]) {
			direction_state = 4;
			anim_frame = frame / 4;
			if (char_box.y < (SCREEN_HEIGHT - SPRITE_SIZE * 3))
				char_box.y += VELOCITY;
		}
		if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_RIGHT]) {
			direction_state = 1;
			anim_frame = frame / 4;
			if ((char_box.y > 0) && (char_box.x < (SCREEN_WIDTH - SPRITE_SIZE * 3))) {
				char_box.x += VELOCITY / 2;
				char_box.y -= VELOCITY / 2;
			}
		}
		if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_LEFT]) {
			direction_state = 5;
			anim_frame = frame / 4;
			if ((char_box.y > 0) && (char_box.x > 0)) {
				char_box.x -= VELOCITY / 2;
				char_box.y -= VELOCITY / 2;
			}
		}
		if (keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_RIGHT]) {
			direction_state = 3;
			anim_frame = frame / 4;
			if ((char_box.y < (SCREEN_HEIGHT - SPRITE_SIZE * 3)) &&
					(char_box.x < (SCREEN_WIDTH - SPRITE_SIZE * 3))) {
				char_box.x += VELOCITY / 2;
				char_box.y += VELOCITY / 2;
			}
		}
		if (keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_LEFT]) {
			direction_state = 7;
			anim_frame = frame / 4;
			if ((char_box.y < (SCREEN_HEIGHT - SPRITE_SIZE * 3)) && (char_box.x > 0)) {
				char_box.x -= VELOCITY / 2;
				char_box.y += VELOCITY / 2;
			}
		}
		if (keys[SDL_SCANCODE_ESCAPE]) {
				quit = true;
		}

		renderTexture(character, renderer, char_box, &char_clips[anim_frame][direction_state]);

		renderTexture(txt_image, renderer, 20, 20, nullptr);

		SDL_RenderPresent(renderer);

		++frame;
		if ( frame / 4 >= ANIMATION_FRAMES )
			frame = 0;

		++countedFrames;
	}


	// Cleanup

	TTF_CloseFont(fps_font);

	SDL_DestroyTexture(bg);
	SDL_DestroyTexture(character);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();
	return 0;
}
