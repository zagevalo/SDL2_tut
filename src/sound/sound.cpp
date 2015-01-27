#include <iostream>
#include <SDL2/SDL_mixer.h>
#include "../logging/logging.h"


Mix_Music* loadBGMusic(const std::string &file) {

	Mix_Music* bgMusic = Mix_LoadMUS(file.c_str());
	if( bgMusic == nullptr )
		logSDLError(std::cout, "Mix_LoadMUS");

	return bgMusic;
}


void playBGMusic(Mix_Music* bgMusic) {
	Mix_PlayMusic(bgMusic, -1);
}


void pauseBGMusic() {
	Mix_PauseMusic();
}


void resumeBGMusic() {
	Mix_ResumeMusic();
}


void unloadBGMusic(Mix_Music* bgMusic) {
	Mix_FreeMusic(bgMusic);
	bgMusic = nullptr;
}
