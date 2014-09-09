#pragma once

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip);

TTF_Font* openFont(const std::string &fontFile, int fontSize);
SDL_Texture* renderText(const std::string &message, TTF_Font* font, SDL_Color color, SDL_Renderer *renderer);

