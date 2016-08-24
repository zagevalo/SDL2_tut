#include <iostream>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "logging/logging.h"
#include "render/render.h"
#include "timer.h"
#include "character/character.h"
#include "sound/sound.h"
#include "SDL_Routine/SDL_Routine.h"

#include "consts.h"


int main(int argc, char **argv) {

	// Init all SDl stuff

	SDL_Routine SDL_hdl;
	if (SDL_hdl.InitSDL()) {
		logSDLError(std::cout, "SDL Init");
		return 1;
	}

	SDL_Texture *bg = loadTexture("res/bg.png", SDL_hdl.Render);
	Character Alice;

	Alice.loadTex("res/Alice_spritesheet.png", SDL_hdl.Render);

	if (bg == nullptr || Alice.char_tex == nullptr)
			return 4;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);


	// Main Loop

	Mix_Music *bg_mus = loadBGMusic("res/bg.mp3");
	playBGMusic(bg_mus);
	bool mus_status = true;

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

		SDL_RenderClear(SDL_hdl.Render);

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
		renderTexture(bg, SDL_hdl.Render, bg_box, nullptr);

		// Show FPS
		txt_image = renderText(fps_text.str().c_str(), fps_font, textColor, SDL_hdl.Render);
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
			Alice.direction_state = 6;
			Alice.anim_frame = Alice.frame / 4;
			if (Alice.char_box.x > 0)
				Alice.char_box.x -= VELOCITY;
		}
		if (keys[SDL_SCANCODE_RIGHT]) {
			Alice.direction_state = 2;
			Alice.anim_frame = Alice.frame / 4;
			if (Alice.char_box.x <= (SCREEN_WIDTH - SPRITE_SIZE * 3))
				Alice.char_box.x += VELOCITY;
		}
		if (keys[SDL_SCANCODE_UP]) {
			Alice.direction_state = 0;
			Alice.anim_frame = Alice.frame / 4;
			if (Alice.char_box.y > 0)
				Alice.char_box.y -= VELOCITY;
		}
		if (keys[SDL_SCANCODE_DOWN]) {
			Alice.direction_state = 4;
			Alice.anim_frame = Alice.frame / 4;
			if (Alice.char_box.y < (SCREEN_HEIGHT - SPRITE_SIZE * 3))
				Alice.char_box.y += VELOCITY;
		}
		if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_RIGHT]) {
			Alice.direction_state = 1;
			Alice.anim_frame = Alice.frame / 4;
			if ((Alice.char_box.y > 0) && (Alice.char_box.x < (SCREEN_WIDTH - SPRITE_SIZE * 3))) {
				Alice.char_box.x += VELOCITY / 4;
				Alice.char_box.y -= VELOCITY / 4;
			}
		}
		if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_LEFT]) {
			Alice.direction_state = 5;
			Alice.anim_frame = Alice.frame / 4;
			if ((Alice.char_box.y > 0) && (Alice.char_box.x > 0)) {
				Alice.char_box.x -= VELOCITY / 4;
				Alice.char_box.y -= VELOCITY / 4;
			}
		}
		if (keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_RIGHT]) {
			Alice.direction_state = 3;
			Alice.anim_frame = Alice.frame / 4;
			if ((Alice.char_box.y < (SCREEN_HEIGHT - SPRITE_SIZE * 3)) &&
					(Alice.char_box.x < (SCREEN_WIDTH - SPRITE_SIZE * 3))) {
				Alice.char_box.x += VELOCITY / 4;
				Alice.char_box.y += VELOCITY / 4;
			}
		}
		if (keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_LEFT]) {
			Alice.direction_state = 7;
			Alice.anim_frame = Alice.frame / 4;
			if ((Alice.char_box.y < (SCREEN_HEIGHT - SPRITE_SIZE * 3)) && (Alice.char_box.x > 0)) {
				Alice.char_box.x -= VELOCITY / 4;
				Alice.char_box.y += VELOCITY / 4;
			}
		}
		if (keys[SDL_SCANCODE_P]) {
			if (mus_status) {
				pauseBGMusic();
				mus_status = false;
			}
			else {
				resumeBGMusic();
				mus_status = true;
			}
		}
		if (keys[SDL_SCANCODE_ESCAPE]) {
			quit = true;
		}

		renderTexture(Alice.char_tex, SDL_hdl.Render, Alice.char_box, &Alice.char_clips[Alice.anim_frame][Alice.direction_state]);

		renderTexture(txt_image, SDL_hdl.Render, 20, 20, nullptr);

		SDL_RenderPresent(SDL_hdl.Render);

		++Alice.frame;
		if ( Alice.frame / 4 >= ANIMATION_FRAMES )
			Alice.frame = 0;

		++countedFrames;
	}


	// Cleanup

	unloadBGMusic(bg_mus);

	TTF_CloseFont(fps_font);

	SDL_DestroyTexture(bg);
	SDL_DestroyTexture(Alice.char_tex);

	SDL_hdl.CleanupSDL();

	return 0;
}
