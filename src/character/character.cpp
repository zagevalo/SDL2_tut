#include <SDL2/SDL.h>
#include "character.h"
#include "../consts.h"

Character::Character() {

	// Init char_box
	char_box.x = SCREEN_WIDTH / 2 - SPRITE_SIZE * 1.5;
	char_box.y = SCREEN_HEIGHT / 2 - SPRITE_SIZE * 1.5;
	char_box.w = SPRITE_SIZE * 3;
	char_box.h = SPRITE_SIZE * 3;

	anim_frame = 0;
	frame = 0;
	direction_state = 4;
	char_tex = nullptr;
}


void Character::move() {

}


void Character::render() {

}
