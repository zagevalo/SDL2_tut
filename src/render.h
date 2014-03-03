#pragma once

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
