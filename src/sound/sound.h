#pragma once

Mix_Music* loadBGMusic(const std::string &file);
void playBGMusic(Mix_Music* bgMusic);
void unloadBGMusic(Mix_Music* bgMusic);
void pauseBGMusic();
void resumeBGMusic();
