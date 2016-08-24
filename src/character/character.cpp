#include <iostream>
#include <SDL2/SDL.h>
#include "character.h"
#include "../consts.h"
#include "../render/render.h"

Character::Character() {

	// Init char_box
	char_box.x = SCREEN_WIDTH / 2 - SPRITE_SIZE * 1.5;
	char_box.y = SCREEN_HEIGHT / 2 - SPRITE_SIZE * 1.5;
	char_box.w = SPRITE_SIZE * 3;
	char_box.h = SPRITE_SIZE * 3;

	for (int i=0; i < 8; i++) {
		for (int j=0; j < 4; j++) {
			char_clips[j][i].x = j * SPRITE_SIZE;
			char_clips[j][i].y = i * SPRITE_SIZE;
			char_clips[j][i].w = SPRITE_SIZE;
			char_clips[j][i].h = SPRITE_SIZE;
		}
	}

	anim_frame = 0;
	frame = 0;
	direction_state = 4;
	char_tex = nullptr;
}

void Character::loadTex(const std::string &file, SDL_Renderer *ren) {
	char_tex = loadTexture(file.c_str(), ren);
}

void Character::move() {

}


void Character::render() {

}
