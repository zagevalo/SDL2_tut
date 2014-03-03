#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "logging.h"
#include "render.h"


SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren) {
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr)
		logSDLError(std::cout, "LoadTexture");

	return texture;
}


void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr) {
	SDL_RenderCopy(ren, tex, clip, &dst);
}


void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr) {
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

	renderTexture(tex, ren, dst, clip);
}
