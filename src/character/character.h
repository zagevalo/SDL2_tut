#pragma once

class Character {
	public:
		const int TILE_SIZE = 40;
		const int SPRITE_SIZE = 36;
		const int ANIMATION_FRAMES = 4;
		const int VELOCITY = 4;

		Character();

		void move();

		void render();

		SDL_Texture *char_tex;
		int anim_frame;
		int frame;
		int direction_state; // 0 - up, 1 - up_right, 2 - right, 3 - right_down, 4 - down,
							 // 5 - down_left, 6 - left, 7 - left_up

		SDL_Rect char_box;

	private:


};
